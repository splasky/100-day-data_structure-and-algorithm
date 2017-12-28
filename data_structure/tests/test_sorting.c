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

static void max_heapify(int array[], int start, int end)
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

static void heap_sort(int array[], int len)
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

static void merge(int n1, const int a1[], int n2, const int a2[], int out[])
{
    int i1 = 0, i2 = 0, iout = 0;

    while (i1 < n1 || i2 < n2) {
        if (i2 >= n2 || ((i1 < n1) && (a1[i1] < a2[i2]))) {
            /* a1[i1] exists and is smaller */
            out[iout++] = a1[i1++];
        } else {
            out[iout++] = a2[i2++];
        }
    }
}

static void merge_sort_inline(int n, const int a[], int out[])
{

    int *a1, *a2;

    if (n < 2) {
        /* 0 or 1 elements is already sorted */
        memcpy(out, a, sizeof(int) * n);
    } else {
        a1 = malloc(sizeof(int) * (n / 2));
        a2 = malloc(sizeof(int) * (n - n / 2));

        merge_sort_inline(n / 2, a, a1);
        merge_sort_inline(n - n / 2, a + n / 2, a2);

        /* merge results */
        merge(n / 2, a1, n - n / 2, a2, out);

        free(a1);
        free(a2);
    }
}

static void merge_sort(int* array, int len)
{
    int* out = calloc(len, sizeof(int));
    merge_sort_inline(len, array, out);
    printf("merge sort:\n");
    print_array(out, len);
    free(out);
}

TEST(sorting)
{
    TEST_SORT(bubble_sort);
    TEST_SORT(insert_sort);
    TEST_SORT(select_sort);
    TEST_SORT(shell_sort);
    TEST_SORT(quick_sort);
    TEST_SORT(heap_sort);
    merge_sort(array, array_size);
    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(sorting);
    return NULL;
}

RUN_TESTS(all_tests);
