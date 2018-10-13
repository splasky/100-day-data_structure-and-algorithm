#include "../include/dbg.h"
#include "../include/my_malloc.h"
#include "unit.h"
#include <stdbool.h>


TEST(test_malloc)
{
    int *array = my_malloc(10);
    unit_assert(array[0] = 10, "Test malloc failed!");
    array = my_realloc(array, 20);
    unit_assert(array[11] = 10, "Test realloc failed!");
    int* array2 = my_calloc(10, sizeof(int));
    array2[9] = 10;
    unit_assert(array[9] != 10, "Test calloc failed!");
    my_free(array);
    my_free(array2);
    return NULL;
}

TEST(all_tests)
{

    unit_suite_start();
    unit_run_test(test_malloc);
    return NULL;
}

RUN_TESTS(all_tests);
