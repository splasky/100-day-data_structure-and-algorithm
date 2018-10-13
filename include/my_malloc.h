#ifndef MYMALLOC_H_FIRBTDGF
#define MYMALLOC_H_FIRBTDGF

#include <stdlib.h>

void *my_malloc(size_t size);
void my_free(void* block);
void *my_calloc(size_t num, size_t nsize);
void *my_realloc(void* block, size_t size);

#endif /* end of include guard: MYMALLOC_H_FIRBTDGF */
