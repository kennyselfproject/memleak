#ifndef NAOVERWRITEMEMORY_H
#define NAOVERWRITEMEMORY_H

#include <iostream>
#include <new>
#include <cstring>
#include <cstdlib>

using namespace std;
#include <malloc.h>
#include "NAMemory.h"
#include "NAMemoryDiag.h"

typedef NAMemory CollHeap ;
typedef bool NABoolean;

/*
  首先自己定义operator new函数，来替代编译器全局默认的operator函数
*/
extern void * operator new(size_t size);
extern void * operator new(size_t size, char* file, int line);
extern void * operator new(size_t size, const std::nothrow_t& e, 
                           char* file, int line);
extern void * operator new(size_t size, CollHeap *h, char* file, int line);
extern void * operator new(size_t size, CollHeap *h, bool failureIsFatal, 
                           char* file, int line);

/*
  首先自己定义operator new函数，来替代编译器全局默认的operator函数
*/
extern void * operator new[](size_t size);
extern void * operator new[](size_t size, char* file, int line);
extern void * operator new[](size_t size, const std::nothrow_t& e, char* file, int line);
extern void * operator new[](size_t size, CollHeap* h, char* file, int line);
extern void * operator new[](size_t size, CollHeap* h, bool failureIsFatal, 
                      char* file, int line);

//覆盖掉全局的operator delete 函数
extern void operator delete(void *ptr);
extern void operator delete(void *ptr, char* file, int line);

//覆盖掉全局的operator delete 函数
extern void operator delete[](void *ptr);
extern void operator delete[](void *ptr, char* file, int line);

// -----------------------------------------------------------------------
// The following overloading new operators are for the classes not 
// derived from NABasicObject. e.g. RW classes, if needs to be allocated
// other than system area, use the overloaded new.
// -----------------------------------------------------------------------


// The following operator delete functions will be called to free memory if
// the initialization throws an exception.  They are needed to remove a
// compiler warning from the .NET 2003 compiler.

// The following operator delete functions will be called to free memory if
// the intialization throws an exception.  They are needed to remove a
// compiler warning from the .NET 2003 compiler.

// -----------------------------------------------------------------------
// The followings are macro defines to delete ( i.e. calling the destructor
// and deallocate space ) certain data from certain heap. ( Since delete
// cannot be overloaded with parameter. )
//
// The reason to use macro defines instead of template inlines is because 
// the destructor cannot be called from template function, i.e.
// template <class C>
// inline void deleteMe(C* p) { p->~C(); } failed
// in ~C is not a member function name. If someone has found a way to let
// it work, it should be changed into template function, then. This is 
// a bug in the compiler, if it ever gets fixed, template function is
// a better approach.
//
// -----------------------------------------------------------------------

// When the compiler supports the destructor in template function, 
// the following template functions should be added, used to allocate
// the objects not derived from NABasicObject, e.g. RW objects.

// template<class T>
// void deleteMe(T* p, NAMemory* h)
// { 
//   p->~p(); 
//   if (h)
//     h->deallocateMemory((void*)p);
//   else
//     ::operator delete((void*)p);
//  }

// NADELETEBASIC(p,h) deletes p from CollHeap h without calling the destructor:
// *p is one of the builtin data types char, int, struct, etc.
// (A better name would be NADELETEBUILTIN or NADELETESCALAR,
// as the "BASIC" in this macro does *not* apply to NA*Basic*Object.)
// example : 
// char* p = new (CmpCommon::contextHeap()) char[10];
// NADELETEBASIC(p,CmpCommon::contextHeap());

#define NADELETEBASIC(p,h) \
  (void) (!(p) || \
          ((NOT_CHECK_NAHEAP(h) ? (h)->deallocateMemory((void*)p) : delete p), 0) )
#define NADELETEBASICARRAY(p,h) \
  (void) (!(p) || \
          ((NOT_CHECK_NAHEAP(h) ? (h)->deallocateMemory((void*)p) : delete [] p), 0))

// NADELETE(p,C,h) deletes p from CollHeap h, and calls p's destructor ~C.
// so the destructor of class C will be called.
// example : 
// ClassA* a = new(CmpCommon::contextHeap()) ClassA;
// NADELETE(a,ClassA,CmpCommon::contextHeap());

#define NADELETE(p,C,h)  \
  (void) (!(p) || \
  	 ((NOT_CHECK_NAHEAP(h) ? ((p)->~C(), (h)->deallocateMemory((void*)p)) : \
           delete p), 0))

// NADELETEARRAY(p,n,C,h) deletes p from CollHeap h, p is an array of class C
// so the destructor of C will be called for each element of p (if not NULL).
// The size of array has to be specified in n. (Sorry about that).
// example :
// ClassA* a = new(CmpCommon::contextHeap()) classA[10];
// NADELETEARRAY(a,10,ClassA,CmpCommon::contextHeap());


#define NADELETEARRAY(p,n,C,h) \
{ if (p) { \
  if NOT_CHECK_NAHEAP(h) \
    { for (C* t=(p)+n-1; t>=p; t--) t->~C(); (h)->deallocateMemory((void*)p); } \
  else delete[] (p); } }

#endif // NAOVERWRITEMEMORY_H
