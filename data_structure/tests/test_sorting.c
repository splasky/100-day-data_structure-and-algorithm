#include "../include/dbg.h"
#include "unit.h"
#include <stdio.h>
#include <stdlib.h>

static int array[] = { 343, 43, 25, 234, 53, 45, 43, 2, 7, 8, 78, 89, 423, 345, 56, 5235,
    23, 52, 35, 36, 46 };

#define array_size (21)
#define TEST_SORT(func)                                                                  \
    shuffle(array, array_size);                                                          \
    func(array, array_size);                                                             \
    unit_assert(array[0] == 2, "failed in " #func);                                      \
    print_array(array, array_size);

static void print_array(int* a, int len)
{
    for (int i = 0; i < len; ++i) {
        printf("%d\t", a[i]);
    }
    printf("\n");
}

static void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

static void shuffle(int* array, int size)
{
    for (int i = size - 1; i > 0; i--) {
        int r = rand() % i;
        swap(&array[i], &array[r]);
    }
}

static void bubble_sort(int* array, int len)
{
    for (int i = len - 1; i > 0; --i) {
        for (int j = 0; j < i; ++j) {
            if (array[j] > array[j + 1]) {
                swap(&array[j], &array[j + 1]);
            }
        }
    }
}

static void insert_sort(int* array, int len)
{
    for (int i = 1; i < len; i++) {
        int curr = array[i];
        int j = 0;
        for (j = i; j > 0 && curr < array[j - 1]; j--) {
            array[j] = array[j - 1];
        }
        array[j] = curr;
    }
}

static void select_sort(int* array, int len)
{
    int min = 0;
    for (int i = 0; i < len; ++i) {
        min = i;
        for (int j = i + 1; j < len; ++j) {
            if (array[j] < array[min]) {
                min = j;
            }
        }

        if (i != min)
            swap(&array[i], &array[min]);
    }
}

static void shell_sort(int* array, int len)
{
    for (int gap = len / 2; gap > 0; gap /= 2) {
        insert_sort(array, len);
    }
}

static inline int partition(int* array, int front, int end)
{
    const int pivot = array[end];
    int i = front - 1;
    for (int j = front; j < end; ++j) {
        if (array[j] < pivot) {
            ++i;
            swap(&array[i], &array[j]);
        }
    }

    ++i;
    swap(&array[i], &array[end]);
    return i;
}

static void quick_sort_inline(int* array, int front, int end)
{
    if (front < end) {
        const int pivot = partition(array, front, end);
        quick_sort_inline(array, front, pivot - 1);
        quick_sort_inline(array, pivot + 1, end);
    }
}

static void quick_sort(int* array, int len) { quick_sort_inline(array, 0, len - 1); }

/* static void heap_sort(int* array, int len);
static void radix_sort(int* array, int len);
static void merge_sort(int* array, int len);
 */

TEST(sorting)
{
    TEST_SORT(bubble_sort);
    TEST_SORT(insert_sort);
    TEST_SORT(select_sort);
    TEST_SORT(shell_sort);
    TEST_SORT(quick_sort);
    /*
    merge_sort(array, len);
    heap_sort(array, len);
    radix_sort(array, len);
    */
    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(sorting);
    return NULL;
}

RUN_TESTS(all_tests);
