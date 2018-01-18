#include "../include/darray.h"
#include "../include/dbg.h"

Darray* Darray_create(size_t element_size, size_t initial_max)
{
    Darray* darray = malloc(sizeof(Darray));
    check_mem(darray);
    check(initial_max > 0, "You must set an initial_max >0");
    darray->max = initial_max;

    darray->contents = calloc(initial_max, sizeof(void*));
    check_mem(darray->contents);

    darray->end = 0;
    darray->element_size = element_size;
    darray->expand_rate = DEFAULT_EXPAND_RATE;

    return darray;
error:
    if (darray)
        free(darray);
    return NULL;
}

void Darray_destroy(Darray* darray)
{
    if (darray) {
        if (darray->contents) {
            free(darray->contents);
        }
        free(darray);
    }
}

void Darray_clear(Darray* darray)
{
    if (darray->element_size > 0) {
        for (int i = 0; i < darray->max; ++i) {
            if (darray->contents[i]) {
                free(darray->contents[i]);
            }
        }
    }
}

static inline int Darray_resize(Darray* darray, size_t new_size)
{
    darray->max = new_size;
    check(darray->max > 0, "The new size must be > 0.");

    void* contents = realloc(darray->contents, darray->max * sizeof(void*));

    check_mem(contents);
    darray->contents = contents;
    return 0;
error:
    return -1;
}

int Darray_expand(Darray* darray)
{
    size_t old_max = darray->max;
    check(Darray_resize(darray, darray->max + darray->expand_rate) == 0,
        "Failed to expand darray to new size: %d",
        darray->max + (int)darray->expand_rate);

    memset(darray->contents + old_max, 0, darray->expand_rate + 1);
    return 0;

error:
    return -1;
}

int Darray_contract(Darray* darray)
{
    int new_size
        = darray->end < (int)darray->expand_rate ? (int)darray->expand_rate : darray->end;

    return Darray_resize(darray, new_size + 1);
}

int Darray_push(Darray* darray, void* data)
{
    darray->contents[darray->end] = data;
    darray->end++;

    if (Darray_count(darray) >= Darray_max(darray)) {
        return Darray_expand(darray);
    }

    return 0;
}

void* Darray_pop(Darray* darray)
{
    check(darray->end - 1 >= 0, "Attempt to pop from empty array");

    void* data = Darray_remove(darray, darray->end - 1);
    darray->end--;

    if (Darray_count(darray) > (int)darray->expand_rate
        && Darray_count(darray) % darray->expand_rate) {
        Darray_contract(darray);
    }

    return data;
error:
    return NULL;
}

void Darray_clear_destroy(Darray* darray)
{
    Darray_clear(darray);
    Darray_destroy(darray);
}

void Darray_set(Darray* darray, int index, void* data)
{
    check(index < darray->max, "darray attempt to get past max");
    if (index > darray->end) {
        darray->end = index;
    }

    darray->contents[index] = data;

error:
    return;
}

void* Darray_get(Darray* darray, int index)
{
    check(index < darray->max, "darray attempt to get past max");

    return darray->contents[index];
error:
    return NULL;
}

void* Darray_remove(Darray* darray, int index)
{
    void* data = darray->contents[index];
    darray->contents[index] = NULL;

    return data;
}

void* Darray_new(Darray* darray)
{
    check(darray->element_size > 0, "Can't use Darray_new on 0 size Darrays");

    return calloc(1, darray->element_size);
error:
    return NULL;
}
