#include <iostream>
#include <new>
#include <cstring>
#include <cstdlib>
#include <malloc.h>

using namespace std;

//#define OLD_IMPL

/*
  首先自己定义operator new函数，来替代编译器全局默认的operator函数
*/
#ifdef OLD_IMPL
void * operator new(size_t size)
{
    cout<<"Global override new ("<<size<<")"<<endl;
#else
char *filepath = NULL;
int linenum = 0;

void * operator new(size_t size, char* file, int line)
{
    cout<<"Global override new ("<<size<<","<<file<<","<<line<<")"<<endl;
#endif
    // void * ptr = malloc(size); //自己调用malloc来分配内存
    // return ptr;
    return (::new char[size]);
    //下面这句话会引起递归的调用，重载operator new之后，::operator new就等于调用自己
    //return ::operator new(size);
}

//重载版本的operator new，该函数默认的是调用 上面的operator new函数
#ifdef OLD_IMPL
void * operator new(size_t size, int flag)
{
    cout<<"Override operator new ("<<size<<","<<flag<<")"<<endl;
    return (::operator new(size));
}
#else
void * operator new(size_t size, int flag, char* file, int line)
{
    cout<<"Override operator new ("<<size<<","<<flag<<","<<file<<","<<line<<")"<<endl;
    return (::operator new(size, file, line));
}
#endif


/*
  首先自己定义operator new函数，来替代编译器全局默认的operator函数
*/
#ifdef OLD_IMPL
void * operator new[](size_t size){
    cout<<"Global override new "<<"["<<size<<"]"<<endl;
    void * ptr = malloc(size); //自己调用malloc来分配内存
    return ptr;
}
#else
void * operator new[](size_t size, char* file, int line){
    cout<<"Global override new "<<"["<<file<<","<<line<<"]"<<endl;

    return (::new char[size] );
    //下面这句话会引起递归的调用，重载operator new之后，::operator new就等于调用自己
    //return ::operator new(size);
}
#endif

//重载版本的operator new，该函数默认的是调用 上面的operator new函数
#ifdef OLD_IMPL
void * operator new[](size_t size, int flag)
{
    cout<<"Override operator new ["<<size<<","<<flag<<"]"<<endl;
    return (::operator new(size));
}
#else
void * operator new[](size_t size, int flag, char* file, int line)
{
    cout<<"Override operator new ["<<size<<","<<flag<<","<<file<<","<<line<<"]"<<endl;
    return (::operator new(size, file, line));
}
#endif

//覆盖掉全局的operator delete 函数
void operator delete(void *ptr){
#ifdef OLD_IMPL
    cout<<"Global override delete"<<endl;
#else
    cout<<"Global override delete ("<<filepath<<","<<linenum<<")"<<endl;
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
#ifdef OLD_IMPL
    cout<<"Override operator delete ("<<flag<<")"<<endl;
#else
    cout<<"Override operator delete ("<<flag<<","<<filepath<<","<<linenum<<")"<<endl;
#endif
    ::operator delete(ptr);
    ptr = NULL;
}
#ifndef OLD_IMPL
void operator delete(void *ptr, int flag, char* file, int line){
    cout<<"Override operator delete ("<<flag<<","<<file<<","<<line<<")"<<endl;
    ::operator delete(ptr, file, line);
    ptr = NULL;
}
#endif

//覆盖掉全局的operator delete 函数
void operator delete[](void *ptr){
#ifdef OLD_IMPL
    cout<<"Global override delete"<<endl;
#else
    cout<<"Global override delete ["<<ptr<<","<<filepath<<","<<linenum<<"]"<<endl;
#endif
    free(ptr);
    ptr = NULL;
}

#ifndef OLD_IMPL
void operator delete[](void *ptr, char* file, int line){
    cout<<"Global override delete ["<<ptr<<","<<file<<","<<line<<"]"<<endl;
    ::operator delete [] (ptr, file, line); 
    ptr = NULL;
}
#endif


/*
  重载版本的operator delete，该函数主要的用途是在构造函数执行不成功的时候，
  调用与new 函数对应的 delete来释放，稍后会有对应的列子来介绍，在这个例子中该函数暂时没用
*/
void operator delete[](void *ptr, int flag){
#ifdef OLD_IMPL
    cout<<"Override operator delete ["<<flag<<"]"<<endl;
#else
    cout<<"Override operator delete ["<<flag<<","<<filepath<<","<<linenum<<"]"<<endl;
#endif
    ::operator delete[](ptr);
    ptr = NULL;
}

#ifndef OLD_IMPL
void operator delete[](void *ptr, int flag, char* file, int line){
    cout<<"Override operator delete ["<<flag<<","<<file<<","<<line<<"]"<<endl;
    ::operator delete[](ptr, file, line);
    ptr = NULL;
}
#endif


#ifndef OLD_IMPL
#define new new((char*)__FILE__, __LINE__)
#define delete filepath=(char*)__FILE__,linenum=__LINE__, delete
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
    delete ptr3;
    cout<<"*********************"<<endl;

    return 0;
}
