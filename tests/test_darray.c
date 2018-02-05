#include "../include/darray.h"
#include "../include/dbg.h"
#include "unit.h"
#include <stdint.h>

static void swap(void** a, void** b)
{
    void* tmp = a;
    a = b;
    b = tmp;
}

static void shuffle(Darray* darray)
{
    for (int i = Darray_count(darray); i > 0; --i) {
        int r = rand() % i;
        swap(darray->contents[i], darray->contents[r]);
    }
}

static int int_compare(int** a, int** b) { return (**a > **b) - (**a < **b); }

void Darray_traverse(Darray* darray, void (*func)(void* value))
{
    for (int i = 0; i < Darray_count(darray); ++i) {
        func(Darray_get(darray, i));
    }
}

TEST(test_darray)
{
    Darray* darray = Darray_create(sizeof(int), 100);
    unit_assert(darray, "Darray create failed.");
    unit_assert(darray->contents, "contents are wrong in darray");
    unit_assert(Darray_count(darray) == 0, "end isn't at the right spot");
    unit_assert(darray->element_size == sizeof(int), "element size is wrong.");
    unit_assert(Darray_max(darray) == 100, "wrong max length on initial size");

    int* val1 = Darray_new(darray);
    unit_assert(val1, "failed to make a new element");
    int* val2 = Darray_new(darray);
    unit_assert(val2, "failed to make a new element");

    Darray_set(darray, 0, val1);
    Darray_set(darray, 1, val2);

    unit_assert(Darray_get(darray, 0) == val1, "Wrong first value.");
    unit_assert(Darray_get(darray, 1) == val2, "Wrong second value.");

    int* val_check = Darray_remove(darray, 0);
    unit_assert(val_check != NULL, "Should not get NULL");
    unit_assert(*val_check == *val1, "Should get the first value");
    unit_assert(Darray_get(darray, 0) == NULL, "Should be gone");
    free(val_check);

    val_check = Darray_remove(darray, 1);
    unit_assert(val_check != NULL, "Should not get NULL");
    unit_assert(*val_check == *val2, "Should get the second value");
    unit_assert(Darray_get(darray, 1) == NULL, "Should be gone");
    free(val_check);

    int old_max = Darray_max(darray);
    Darray_expand(darray);
    unit_assert((unsigned int)darray->max == old_max + darray->expand_rate,
        "Wrong size after expand");
    Darray_contract(darray);
    unit_assert((unsigned int)darray->max == darray->expand_rate + 1,
        "Should stay at the expand_rate at least");

    for (int i = 0; i < 1000; ++i) {
        int* val = Darray_new(darray);
        *val = i;
        Darray_push(darray, val);
    }
    unit_assert(darray->max == 1201, "Wrong max size");

    for (int i = 999; i >= 0; i--) {
        int* val = Darray_pop(darray);
        unit_assert(val != NULL, "Shouldn't get a NULL");
        unit_assert(*val == i, "Wrong value.");
        free(val);
    }

    Darray_destroy(darray);

    darray = Darray_create(sizeof(int), 100);

    for (int i = 0; i < 1000; ++i) {
        int* val = Darray_new(darray);
        *val = i;
        Darray_push(darray, val);
    }

    shuffle(darray);
    Darray_qsort(darray, (Darray_compare)int_compare);

    for (int i = 0; i < Darray_count(darray); ++i) {
        unit_assert(*(int*)Darray_get(darray, i) == i, "Darray_qsort failed");
    }

    /* TODO:Fix memory leak */
    Darray_destroy(darray);
    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(test_darray);
    return NULL;
}

RUN_TESTS(all_tests);
