#ifndef NAMEMORYDIAG_H
#define NAMEMORYDIAG_H

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

extern MemHeader_t gGlobalMemory;

extern void *
memory_insert(
        MemHeader_t *pMemHeader, 
        MemList_t *pMemList, 
        size_t size, char *file, int line);

extern void *
memory_delete(MemHeader_t *pMemHeader, void *ptr);

extern void
memory_scan(MemHeader_t *pMemHeader);


#endif // NAMEMORYDIAG_H
