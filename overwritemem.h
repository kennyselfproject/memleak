#ifndef OVERWRITEMEM_H
#define OVERWRITEMEM_H

#include <iostream>
#include <new>
#include <cstring>
#include <cstdlib>

using namespace std;
#include <malloc.h>
#include "memleakdiag.h"

/*
  首先自己定义operator new函数，来替代编译器全局默认的operator函数
*/
extern void * operator new(size_t size, char* file, int line);

/*
  首先自己定义operator new函数，来替代编译器全局默认的operator函数
*/
extern void * operator new[](size_t size, char* file, int line);

//覆盖掉全局的operator delete 函数
extern void operator delete(void *ptr);

extern void operator delete(void *ptr, char* file, int line);

//覆盖掉全局的operator delete 函数
extern void operator delete[](void *ptr);

extern void operator delete[](void *ptr, char* file, int line);

#define new new((char*)__FILE__, __LINE__)

#endif // OVERWRITEMEM_H
