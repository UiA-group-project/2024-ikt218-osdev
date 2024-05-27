#ifndef MALLOC_H
#define MALLOC_H

#include "stdint.h"

#define MAX_PAGE_ALIGNED_ALLOCS 32

void init_kernel_memory(uint32_t *kernel_end);
void print_memory_layout();
void free(void *mem);
void pfree(void *mem);
char *pmalloc(size_t size);
void *malloc(size_t size);

#endif // MALLOC_H