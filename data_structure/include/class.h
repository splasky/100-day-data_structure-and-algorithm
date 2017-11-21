#ifndef OBJECT_H_4CRNBTM9
#define OBJECT_H_4CRNBTM9

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Class_t {
    size_t size;

    void* (*Constructor)(void* self, va_list* valist);
    void* (*Destructor)(void* self);
    void* (*Clone)(const void* self);
    int (*Differ)(const void* self, const void* b);
} Class;

#endif /* end of include guard: OBJECT_H_4CRNBTM9 */
