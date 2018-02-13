#include <iostream>
#include <new>
#include <cstring>
#include <cstdlib>
#include <malloc.h>

using namespace std;

#define MEM_LINK_FILE_NAME_LEN 9
typedef unsigned short int uint2;
typedef unsigned int       uint4;

// the first two attribute must be prev and next, it's as same as MemHeader
typedef struct MemList
{
    struct MemList *prev;       // point to previous memory block
    struct MemList *next;       // point to next memory block
    uint4  size;                // the size of this memory block, include this header
    uint2  line;                // the line of allocate
    char   filename[MEM_LINK_FILE_NAME_LEN]; // the tail 9 bytes file name of allocate
    char   filetype;                         // the file type, it's .c or .h
}MemList_t;

// the first two attribute must be prev and next, it's as same as MemList
typedef struct MemHeader
{
    struct MemList *prev;       // point to previous memory block, must be NULL
    struct MemList *next;       // point to next memory block
    size_t          lstLen;     // total memory is allocated by user
    size_t          total;      // total memory is allocated, include managing memory 
}MemHeader_t;

MemHeader_t gGlobalMemory = {NULL, NULL, NULL, 0};

//#define OLD_IMPL

void *
memory_insert(
        MemHeader_t *pMemHeader, 
        MemList_t *pMemList, 
        size_t size, char *file, int line)
{
    pMemList->line = line;
    pMemList->size = size;
    char *fileEnd = strstr(file, ".");
    if (fileEnd == NULL)
    {
        cout << "ERROR: Don't supported file without type:" << file << endl;
        return NULL;
    }

    pMemList->filetype = fileEnd[1];
    switch(fileEnd[1]) 
    {
    case 'H':
    case 'h':
        cout << "WARNING: header file call a new operator!!!!!" << endl;
    case 'C':
    case 'c':
        {
            int   fileNameLen = fileEnd - file;
            char *fileNameStart = fileEnd - MEM_LINK_FILE_NAME_LEN;
            // skip the .c, .C, .H, .h
            if (fileNameLen > MEM_LINK_FILE_NAME_LEN)
                fileNameLen = MEM_LINK_FILE_NAME_LEN;
            if (fileNameStart < file)
                fileNameStart = file;
                
            memcpy(pMemList->filename, fileNameStart, fileNameLen);
            break;
        }

    default:
        cout << "ERROR: Don't supported file type:" << file << endl;
    }

    // Link MemList to Head of MemHeader
    pMemList->prev = (MemList_t*)pMemHeader;
    pMemList->next = pMemHeader->next;
    pMemHeader->next = pMemList;
    pMemHeader->total += size;
    pMemHeader->lstLen++;

    return (pMemList + 1);
}

void *memory_delete(MemHeader_t *pMemHeader, void *ptr)
{
    MemList_t *pMemList = (MemList_t*)ptr - 1;

    // Delete MemList from MemList list
    pMemList->prev->next = pMemList->next;
    if (pMemList->next != NULL)
        pMemList->next->prev = pMemList->prev;
    pMemHeader->total -= pMemList->size;
    pMemHeader->lstLen--;
    
    return ((void*)pMemList);
}

void memory_scan(MemHeader_t *pMemHeader)
{
    MemList_t *pCur;

    cout << "Memory Header:" << pMemHeader 
         << " prev:" << pMemHeader->prev 
         << " next:" << pMemHeader->next 
         << " lstLen:" << pMemHeader->lstLen
         << " total:" << pMemHeader->total << endl;
    for (pCur = pMemHeader->next; pCur != NULL; pCur = pCur->next) 
    {
        cout << "Memory Leak in [ File:" 
             << pCur->filename << ", Line:" 
             << pCur->line << ", Size:" 
             << pCur->size << "]" 
             << " prev:" << pCur->prev 
             << " next:" << pCur->next << endl;
    }
}

/*
  首先自己定义operator new函数，来替代编译器全局默认的operator函数
*/
#ifdef OLD_IMPL
void * operator new(size_t size)
{
    cout << "Global override new (" << size << ")" <<endl;
    return (::new char[size]);
}
#else
void * operator new(size_t size, char* file, int line)
{
    MemList_t *pMemList = NULL;
    size += sizeof(MemList_t);
    pMemList = (MemList_t*)(::new char[size]);
    cout << "Global override new (" 
         << (char*)pMemList << "," << size << "," << file << "," << line 
         << ")" << endl;

    return memory_insert(&gGlobalMemory, pMemList, size, file, line);
    //下面这句话会引起递归的调用，重载operator new之后，::operator new就等于调用自己
    //return ::operator new(size);
}
#endif

//重载版本的operator new，该函数默认的是调用 上面的operator new函数
#ifdef OLD_IMPL
void * operator new(size_t size, int flag)
{
    cout << "Override operator new (" << size << "," << flag << ")" << endl;
    return (::operator new(size));
}
#else
void * operator new(size_t size, int flag, char* file, int line)
{
    cout << "Override operator new ("
         << size << "," << flag << "," << file << "," << line 
         << ")" << endl;

    return (::operator new(size, file, line));
}
#endif


/*
  首先自己定义operator new函数，来替代编译器全局默认的operator函数
*/
#ifdef OLD_IMPL
void * operator new[](size_t size){
    cout << "Global override new " << "[" << size << "]" << endl;
    void * ptr = malloc(size); //自己调用malloc来分配内存
    return ptr;
}
#else
void * operator new[](size_t size, char* file, int line){
    MemList_t *pMemList = NULL;
    size += sizeof(MemList_t);
    pMemList = (MemList_t*)(::new char[size]);
    cout << "Global override new [" 
         << (char*)pMemList << "," << size << "," << file << "," << line 
         << "]" << endl;

    return memory_insert(&gGlobalMemory, pMemList, size, file, line);
    //下面这句话会引起递归的调用，重载operator new之后，::operator new就等于调用自己
    //return ::operator new(size);
}
#endif

//重载版本的operator new，该函数默认的是调用 上面的operator new函数
#ifdef OLD_IMPL
void * operator new[](size_t size, int flag)
{
    cout << "Override operator new [" << size << "," << flag << "]" << endl;

    return (::operator new(size));
}
#else
void * operator new[](size_t size, int flag, char* file, int line)
{
    cout << "Override operator new ["
         << size << "," << flag << "," << file << "," << line 
         <<"]"<<endl;
    return (::operator new(size, file, line));
}
#endif

//覆盖掉全局的operator delete 函数
void operator delete(void *ptr){
    cout <<"Global override delete: "<< ptr << endl;
#ifndef OLD_IMPL
    ptr = memory_delete(&gGlobalMemory, ptr);
#endif
    free(ptr);
    ptr = NULL;
}
#ifndef OLD_IMPL
void operator delete(void *ptr, char* file, int line){
    cout<<"Global override delete ("<<ptr<<","<<file<<","<<line<<")"<<endl;
    ::operator delete(ptr);
    ptr = NULL;
}
#endif


/*
  重载版本的operator delete，该函数主要的用途是在构造函数执行不成功的时候，
  调用与new 函数对应的 delete来释放，稍后会有对应的列子来介绍，在这个例子中该函数暂时没用
*/
void operator delete(void *ptr, int flag){
    cout << "Override operator delete (" << ptr << "," << flag << ")" << endl;
#ifndef OLD_IMPL
    ptr = memory_delete(&gGlobalMemory, ptr);
#endif
    ::operator delete(ptr);
    ptr = NULL;
}

#ifndef OLD_IMPL
void operator delete(void *ptr, int flag, char* file, int line){
    cout << "Override operator delete ("
         << ptr << "," << flag << "," << file << "," << line 
         << ")" << endl;
    ::operator delete(ptr, file, line);
    ptr = NULL;
}
#endif

//覆盖掉全局的operator delete 函数
void operator delete[](void *ptr){
    cout<<"Global override delete:" << ptr << endl;
#ifndef OLD_IMPL
    ptr = memory_delete(&gGlobalMemory, ptr);
#endif
    free(ptr);
    ptr = NULL;
}

#ifndef OLD_IMPL
void operator delete[](void *ptr, char* file, int line){
    cout << "Global override delete ["
         << ptr << "," << file << "," << line 
         << "]" << endl;
    ::operator delete [] (ptr, file, line); 
    ptr = NULL;
}
#endif


/*
  重载版本的operator delete，该函数主要的用途是在构造函数执行不成功的时候，
  调用与new 函数对应的 delete来释放，稍后会有对应的列子来介绍，在这个例子中该函数暂时没用
*/
void operator delete[](void *ptr, int flag){
    cout << "Override operator delete [" << flag << "]" << endl;
    ::operator delete[](ptr);
    ptr = NULL;
}

#ifndef OLD_IMPL
void operator delete[](void *ptr, int flag, char* file, int line){
    cout << "Override operator delete ["
         << flag << "," << file << "," << line 
         <<"]"<<endl;
    ::operator delete[](ptr, file, line);
    ptr = NULL;
}
#endif


#ifndef OLD_IMPL
#define new new((char*)__FILE__, __LINE__)
#endif

class test
{
public:
    test(int a) {a_ = a;}

private:
    int a_;
};

int main(){
    int * ptr1 = new int(10);

    int * ptr2 = new int[10];
    test * ptr3 = new test(10);

    /*
      delete ptr; 调用的就是 void operator delete(void * ptr); 而与new 匹配的delete 不是自己调用的
      ，而是在new申请,成功却在构造函数时候出错，new operator自己根据operator new 来寻找 
      对应的operator delete 来调用，稍后介绍。
    */
    delete ptr1;
    delete [] ptr2;
    if (ptr3 == NULL)
        return -1;
    //delete ptr3;
    cout<<"*********************"<<endl;

    memory_scan(&gGlobalMemory);

    return 0;
}
