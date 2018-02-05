#include "../include/new.h"
#include "../include/class.h"
#include "../include/dbg.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void* New(const void* _class, ...)
{
    const Class* class = _class;
    void* p = calloc(1, class->size);
    check_mem(p);
    *(const Class**)p = class;

    if (class->Constructor) {
        va_list ap;
        va_start(ap, _class);
        p = class->Constructor(p, &ap);
        va_end(ap);
    }

    return p;

error:
    log_err("Error in New.");
    return NULL;
}

void Del(void* self)
{
    const Class** cp = self;

    if (self && *cp && (*cp)->Destructor) {
        self = (*cp)->Destructor(self);
        free(self);
        self = NULL;
    }
}

void* Clone(const void* self)
{
    const Class* const* cp = self;
    assert(self && *cp && (*cp)->Clone);
    return (*cp)->Clone(self);
}

int Differ(const void* self, const void* b)
{
    const Class* const* cp = self;
    assert(self && *cp && (*cp)->Differ);
    return (*cp)->Differ(self, b);
}

size_t sizeOf(const void* self)
{
    const Class* const* cp = self;
    assert(self && *cp);
    return (*cp)->size;
}
