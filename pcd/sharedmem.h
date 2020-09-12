#ifndef _SHARED_MEM_H
#define _SHARED_MEM_H

#include <stdlib.h>

#define FILENAME "shmfile"
#define BL_SIZE 4096

char*
attach_memblock(const char* fname, size_t size);

int
detach_memblock(char* block);

int
destroy_memblock(char* fname);

#endif /* _SHARED_MEM_H */
