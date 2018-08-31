#include "../include/dbg.h"
#include "../include/sort.h"
#include "unit.h"
#include <stdio.h>
#include <stdlib.h>

static int array[] = { 343, 43, 25, 234, 53, 45, 43, 2, 7, 8, 78, 89, 423, 345, 56, 5235,
                       23, 52, 35, 36, 46
                     };

#define array_size (21)
#define TEST_SORT(func)                                        \
    shuffle(&array, array_size,sizeof(int));                   \
    func(&array, array_size,sizeof(int),compare);              \
    print_array(array, array_size,sizeof(int),print);          \
    unit_assert(array[0] == 2, "failed in " #func);


void print(void* a)
{
    printf("%d ", *(int*)a);
}

static int compare(const void* a, const void* b)
{
    const int32_t *va = (int*)a;
    const int32_t *vb = (int*)b;
    if (*va > *vb)
    {
        return 1;
    }
    else if (*va < *vb)
    {
        return -1;
    }
    return 0;
}

TEST(sorting)
{
    TEST_SORT(bubble_sort);
    TEST_SORT(insert_sort);
    TEST_SORT(select_sort);
    TEST_SORT(shell_sort);
    TEST_SORT(quick_sort);
    TEST_SORT(heap_sort);
    TEST_SORT(merge_sort);
    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(sorting);
    return NULL;
}

RUN_TESTS(all_tests);
