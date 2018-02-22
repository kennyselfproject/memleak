#include <iostream>
#include <new>
#include <cstring>
#include <cstdlib>

using namespace std;
#include <malloc.h>
#include "NAOverWriteMemory.h"

bool gEnableMemoryDiag = true;

/*
  首先自己定义operator new函数，来替代编译器全局默认的operator函数
*/
void * operator new(size_t size)
{
    return malloc(size);
}

void * operator new(size_t size, char* file, int line)
{
    if (gEnableMemoryDiag)
    {
        MemList_t *pMemList = NULL;
        size += sizeof(MemList_t);
        pMemList = (MemList_t*)(::new char[size]);
        cout << "Global override new (" 
             << pMemList << "," << size << "," << file << "," << line 
             << ")" << endl;

        return memory_insert(&gGlobalMemory, pMemList, size, file, line);
    }
    else 
    {
        return (::new char[size]);
    }
}

void * operator new(size_t size, const std::nothrow_t& e, char* file, int line)
{
    if (gEnableMemoryDiag)
    {
        MemList_t *pMemList = NULL;
        size += sizeof(MemList_t);
        pMemList = (MemList_t*)(::new char[size]);
        cout << "Global override new (" 
             << pMemList << "," << size << "," << file << "," << line 
             << ")" << endl;

        return memory_insert(&gGlobalMemory, pMemList, size, file, line);
    }
    else 
    {
        return (::new(e) char[size]);
    }
}

void* operator new (size_t t, CollHeap* h, char* file, int line)
{

  if (NOT_CHECK_NAHEAP(h))
  {
    return h->allocateMemory(t);
  } else {

#ifdef NA_LEAK_DETECTION
      get_caller_location();
#endif
      return (::operator new(t, file, line));
  }

}

void* operator new (size_t t, CollHeap* h, bool failureIsFatal, 
                    char* file, int line)
{
  if (NOT_CHECK_NAHEAP(h))
  {
    return h->allocateMemory(t, failureIsFatal);
  } else {

#ifdef NA_LEAK_DETECTION
    get_caller_location();
#endif

    return (::operator new(t, file, line));
  }

}

void * operator new[](size_t size)
{
    return malloc(size);
}

/*
  首先自己定义operator new函数，来替代编译器全局默认的operator函数
*/
void * operator new[](size_t size, char* file, int line)
{
    if (gEnableMemoryDiag)
    {
        MemList_t *pMemList = NULL;
        size += sizeof(MemList_t);
        pMemList = (MemList_t*)(::new char[size]);
        cout << "Global override new [" 
             << pMemList << "," << size << "," << file << "," << line 
             << "]" << endl;

        return memory_insert(&gGlobalMemory, pMemList, size, file, line);
    }
    else 
    {
        return (::new char[size]);
    }
    //下面这句话会引起递归的调用，重载operator new之后，::operator new就等于调用自己
    //return ::operator new(size);
}

void * operator new[](size_t size, const std::nothrow_t& e, char* file, int line)
{
    if (gEnableMemoryDiag)
    {
        MemList_t *pMemList = NULL;
        size += sizeof(MemList_t);
        pMemList = (MemList_t*)(::new char[size]);
        cout << "Global override new [" 
             << pMemList << "," << size << "," << file << "," << line 
             << "]" << endl;

        return memory_insert(&gGlobalMemory, pMemList, size, file, line);
    }
    else 
    {
        return (::new(e) char[size]);
    }
    //下面这句话会引起递归的调用，重载operator new之后，::operator new就等于调用自己
    //return ::operator new(size);
}


void* operator new[] (size_t t, CollHeap* h, char* file, int line)
{
  //return ( (h) ? h->allocateMemory(t) : (::new char[t] ) );

  if (NOT_CHECK_NAHEAP(h))
  {
    return h->allocateMemory(t);
  } else {

#ifdef NA_LEAK_DETECTION
    get_caller_location();
#endif

    return (::operator new(t, file, line));
  }

}

void* operator new[] (size_t t, CollHeap* h, bool failureIsFatal, 
                      char* file, int line)
{
  if (NOT_CHECK_NAHEAP(h))
  {
    return h->allocateMemory(t, failureIsFatal);
  } else {

#ifdef NA_LEAK_DETECTION
    get_caller_location();
#endif

    return (::operator new(t, file, line));
  }

}


//覆盖掉全局的operator delete 函数
void operator delete(void *ptr){
    ptr = memory_delete(&gGlobalMemory, ptr);
    cout <<"Global override delete: "<< ptr << endl;
    free(ptr);
    ptr = NULL;
    }

void operator delete(void *ptr, char* file, int line){
    cout<<"Global override delete ("<<ptr<<","<<file<<","<<line<<")"<<endl;
    ::operator delete(ptr);
    ptr = NULL;
}

//覆盖掉全局的operator delete 函数
void operator delete[](void *ptr){
    ptr = memory_delete(&gGlobalMemory, ptr);
    cout<<"Global override delete:" << ptr << endl;
    free(ptr);
    ptr = NULL;
}

void operator delete[](void *ptr, char* file, int line){
    cout << "Global override delete ["
         << ptr << "," << file << "," << line 
         << "]" << endl;
    ::operator delete [] (ptr, file, line); 
    ptr = NULL;
}

