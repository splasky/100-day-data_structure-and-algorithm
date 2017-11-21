#ifndef NEW_H_GN4ONI7Q
#define NEW_H_GN4ONI7Q

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void* New(const void* _class, ...);
void Del(void* self);
void* Clone(const void* self);
int Differ(const void* self, const void* b);
size_t sizeOf(const void* self);

#endif /* end of include guard: NEW_H_GN4ONI7Q */
