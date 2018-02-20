using namespace std;
#include "NAMemoryDiag.h"

MemHeader_t gGlobalMemory = {NULL, NULL, 0, 0};

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
    if (pMemHeader->next != NULL)
        pMemHeader->next->prev = pMemList;
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
    switch(pMemList->filetype) 
    {
    case 'H':
    case 'h':
        cout << "WARNING: header file call a new operator!!!!!" << endl;
    case 'C':
    case 'c':
        break;

    default:
        return ptr;
    }

    if (pMemList->prev != NULL)
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
             << pCur->size - sizeof(MemList_t) << "]"
             << " cur:" << pCur
             << " prev:" << pCur->prev 
             << " next:" << pCur->next << endl;
    }

    cout << endl;
}
