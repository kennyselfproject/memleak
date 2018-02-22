/**********************************************************************
// @@@ START COPYRIGHT @@@
//
// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
// @@@ END COPYRIGHT @@@
**********************************************************************/
/* -*-C++-*-
****************************************************************************
*
* File:         NABasicObject.h (previously under /common)
* Description:  This file contains the declaration of NABasicObject, which
*               has the new/delete defined to use CollHeap* as the memory
*               allocation.
*
* Created:      5/6/98
* Language:     C++
*
*
*
****************************************************************************
*/

#ifndef NABASICOBJECT__H
#define NABASICOBJECT__H

#include <iostream>
#include <new>
#include <cstring>
#include <cstdlib>

#include "Platform.h"
#include "NAMemory.h"
#include "NAOverWriteMemory.h"

/*
#include "NAStdlib.h"
#include "NABoolean.h"

#define NOT_CHECK_NAHEAP(h) (h)

class NAMemory ;
typedef NAMemory CollHeap ;

// -----------------------------------------------------------------------
// This file contains the 
// NABasicObject class for the new/delete operator for memory management, and
// memory management macros, overloaded new operators for the classes 
// not derived from NABasicObject.
//
// -class CollHeap - defined in NAHeap.h,  abstract base class.
//    allocateMemory, deallocateMemory methods defined.
//
// -class NABasicObject
//    A base class for all classes that want to use new/delete from CollHeap*
//    It contains the implementation for overloaded new/delete operators.
//
// -examples:
//   class YourHeap : public CollHeap
//     implement your allocateMemory and deallocateMemory methods.
//
//   class YourClass : NABasicObject
//
//   then, you can do
//   YourHeap* aHeap = new YourHeap;
//   YourClass* object = new (aHeap) YourClass(aHeap);
//     the memory  will be allocated through YourHeap
//   YourClass* sysObject = new YourClass;
//     the memory will be allocated through global new
//   delete object;
//   delete sysObject;
//     the memory allocated will be freed accordingly.
//
// - Some new operators and macro defines (for delete) for CollHeap*
//   memory management.
//   These are for classes/types that can NOT be derived from NABasicObject.
//   e.g. RW classes, basic types, .etc. Also for the new of array which
//   goes through the global new/delete always.
//     void * operator new(size_t size, CollHeap* h);
//     NADELETEBASIC  (which despite its name does NOT delete NABasicObjects)
//     NADELETE	      (in fact, neither does this)
//     NADELETEARRAY  (or this)
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
//
// Examples to use them :
//
// 1. Derive your class from NABasicObject 
//    (Strongly suggested for the newly developed class)
//    example :
//    class A : public NABasicObject { }
//
//     A* a = new (anyCollHeap*) A;
//     delete a;
//   
//   The memory will be allocated/deallocated from the specified CollHeap*.
//
//     A* asys = new A;
//     delete asys;
// 
//   asys will be allocateed/deallocated using the global new/delete operator.
//
// 2. For the existing classes, you can do
//    class B;
//    B* b = new(anyCollHeap*);
//    NADELETE(b,B,anyCollHeap*);
//
// 3. For builtin types, in which there is no need to call a destructor, do
//    int* p = new (anyCollHeap*) int[100];
//    NADELETEBASIC(p,anyCollHeap);	// better name would be NADELETEBUILTIN
//					// or NADELETESCALAR ...
//
// 4. For array of elements, do
//    class C;
//    C* c = new(anyCollHeap*) C[100];
//    NADELETEARRAY(c,100, C,anyCollHeap*);
//
//    If the array of elements are allocated from CollHeap*, it has to be
//    deleted using NADELETEARRAY, because the new of array will always
//    go through the global new operator.
//    You can only do delete[] array if it is newed from global new
//    operator.
//
// Note that global new/delete will still work fine, but just get the memory
// from global new/delete operator.
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// NABasicObject overloads the new/delete operators to use CollHeap* as
// memory management for classes derived from it.
// -----------------------------------------------------------------------

class NABasicObject
{

protected:
 virtual ~NABasicObject() {};

public:

  // The destructor is not a virtual function, because all the executor
  // objects are derived from NABasicObject, and the objects will be
  // written to the disk. To save space in the disk, the destructor is
  // not a virtual function. So, NABasicObject* cannot used for delete
  // it will not call the correct destructor.
  //
  // The destructor is the default destructor, which does NOT (of course)
  // do "delete h_;"!  Likewise, the assignment operator here does not
  // overwrite the h_ pointer.

  NABasicObject();
  NABasicObject(const NABasicObject&);
  NABasicObject& operator=(const NABasicObject&);

  void* operator new[](size_t t, NAMemory* h = 0,  NABoolean failureIsFatal = TRUE);
  void operator delete[](void*);
  void operator delete[](void*, NAMemory*, NABoolean);

  // In the future, when the compiler supports the operator new[] and
  // operator [] delete, they should be added here for array allocation.
  // void* operator new[](size_t, NAMemory* h = 0,  NABoolean failureIsFatal = TRUE);
  // void operator delete[](void*, etc... -- see .C file comments...)

  void* operator new(size_t t, NAMemory* h = 0, NABoolean failureIsFatal = TRUE );
  void* operator new(size_t t, void* loc, NAMemory* h = 0);

  void operator delete(void*);
  void operator delete(void*, NAMemory*, NABoolean); 
  void operator delete(void*, void*, NAMemory*); 

  // The CollHeap* returned is where this object is allocated from;
  // if this object is allocated through new, it will be some CollHeap* or 0.
  // If the object is a local stack variable, the CollHeap* will be
  // uninitialized garbage. 
  // In the design of the derived classes, if you want to use this
  // CollHeap* as further memory allocation, it is the user's
  // responsibility to make sure the object is not from stack.

  NAMemory* collHeap()	      { return h_; }

  // The delete operator sets the object-being-deleted's heap pointer to the
  // invalid value below.  Thus dangling pointers to this object can *sometimes*
  // be identified by use of this eyecatcher when in the debugger, or 
  // smart-pointer code can explicitly check for this value, in the cases where:
  // - the pointed-to memory has not actually been freed (deallocateMemory method
  //   is often a no-op, so this is common); or
  // - the memory has been freed but happens to have been reallocated with a
  //   new object before the dangling pointer referenced it; or
  // - the memory has been freed and the dangling ptr reference occurs in an
  //   environment that allows freed memory accesses (SunOS Unix, e.g.).
  // Note that the first two cases cannot be found even running Purify.
  // However, note that "*sometimes*":  if the pointed-at object was not
  // allocated by "new" but rather on the stack, then its h_ is (as said before)
  // random garbage, which *could* be the magic "invalid" value.
  // This problem could be solved with an additional "isValid_/isAllocated_"
  // flag word, but that would be an additional executor disk space hit ...
  //
  // The value of 0x1 was chosen because low memory is on most machines going
  // to be out-of-bounds for a heap.  (0x0 already has a meaning: that no heap
  // is to be used, just the global new/delete.)

  static CollHeap* invalidHeapPtr() { return (CollHeap*)0x1; }
  static CollHeap* systemHeapPtr()  { return (CollHeap*)0x0; }
  Int32 maybeInvalidObject()	       { return h_ == invalidHeapPtr();}

  // For smart-pointer callers, some defensive programming for Debug build.
#if !defined(NDEBUG)
    Int32 checkInvalidObject(const void* const referencingObject=NULL);
#else
    Int32 checkInvalidObject(const void* const = NULL) 
					       { return 0; }
#endif

private:
  // To add a field into the NABasicObject in later releases,
  // need to have versioning in the generated rtd files. Since 
  // the size will be different. 

  CollHeap* h_;

};
*/

#include "NAOverWriteMicro.h"

#endif
