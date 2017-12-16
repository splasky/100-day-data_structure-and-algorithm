#include "../include/dbg.h"
#include "unit.h"
#include <stdio.h>

void swap(int* a, int* b)
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

void max_heapify(int array[], int start, int end)
{
    int dad = start;
    int son = (dad << 1) + 1;

    while (son <= end) {
        if (son + 1 <= end && array[son] < array[son + 1])
            son++;
        if (array[dad] > array[son]) {
            return;
        } else {
            swap(&array[dad], &array[son]);
            dad = son;
            son = (dad << 1) + 1;
        }
    }
}

void heapSort(int array[], int len)
{
    /* change array to heap */
    for (int i = len / 2 - 1; i >= 0; i--) {
        max_heapify(array, i, len - 1);
    }

    /* change root with last leaf */
    for (int i = len - 1; i > 0; i--) {
        swap(&array[0], &array[i]);
        max_heapify(array, 0, i - 1);
    }
}

TEST(test_heap_sort)
{
    int array[] = { 1, 3, 5, 56, 67, 37, 234, 25, 654, 34, 95, 34, 45, 6, 7, 73, 5, 4, 3,
        234, 5, 654, 34 };
    int len = sizeof(array) / sizeof(array[0]);

    heapSort(array, len);
    for (int i = 0; i < len; ++i) {
        printf("%d\t", array[i]);
    }
    printf("\n");

    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(test_heap_sort);
    return NULL;
}

RUN_TESTS(all_tests);
