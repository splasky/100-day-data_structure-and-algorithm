#ifndef DARRAY_H_BLDW2HAP
#define DARRAY_H_BLDW2HAP

#include <stdlib.h>

typedef struct darray Darray;
struct darray {
    int end;
    int max;
    size_t element_size;
    size_t expand_rate;
    void** contents;
};

Darray* Darray_create(size_t element_size, size_t initial_max);
void Darray_destroy(Darray* darray);
void Darray_clear(Darray* darray);
int Darray_expand(Darray* darray);
int Darray_contract(Darray* darray);
int Darray_push(Darray* darray, void* data);
void* Darray_pop(Darray* darray);
void Darray_clear_destroy(Darray* darray);
void Darray_set(Darray* darray, int index, void* data);
void* Darray_get(Darray* darray, int index);
void* Darray_remove(Darray* darray, int index);
void* Darray_new(Darray* darray);

#define Darray_last(A) ((A)->contains[(A)->end - 1])
#define Darray_first(A) ((A)->contains[0])
#define Darray_count(A) ((A)->end)
#define Darray_max(A) ((A)->max)
#define DEFAULT_EXPAND_RATE 300

#endif /* end of include guard: DARRAY_H_BLDW2HAP */
