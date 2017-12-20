#include "../include/dbg.h"
#include "unit.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define N (1024)

void swap(int* a, int* b)
{
    int temp = *a;
    *a=*b;
    *b=temp;
}

void shuffle(int size, int* array)
{
    for (int i = size - 1; i > 0; i--) {
        int r = rand() % i;
        swap(&array[i], &array[r]);
    }
}

static int splitByPivot(int n, int* a, int pivot)
{
    int low = 0;
    int hi = n - 1;

    assert(n >= 0);

    while (low <= hi) {
        if (a[low] <= pivot) {
            low++;
        } else {
            swap(&a[hi],&a[low]);
            hi--;
        }
    }

    return low;
}

int quickselectDestructive(int k, int n, int* a)
{
    assert(0 <= k);
    assert(k < n);

    if (n == 1) {
        return a[0];
    }

    int pivot = a[rand() % n];
    int lo = splitByPivot(n, a, pivot);

    if (k < lo) {
        return quickselectDestructive(k, lo, a);
    } else {
        return quickselectDestructive(k - lo, n - lo, a + lo);
    }
}

void quickSort(int size, int* a)
{
    int pivot;
    int low = 0;

    if (size <= 1)
        return;

    /* else */

    pivot = a[rand() % size];
    low = splitByPivot(size, a, pivot);

    quickSort(low, a);
    quickSort(size - low, a + low);
}

TEST(test_quicksort_and_quickselect)
{
    int a[N];

    srand(0);
    for (int i = 0; i < N; ++i) {
        a[i] = i;
    }

    shuffle(N, a);

    for (int i = 0; i < N; ++i) {
        unit_assert(quickselectDestructive(i, N, a) == i, "select failed");
    }

    shuffle(N, a);

    quickSort(N, a);
    for (int i = 0; i < N; ++i) {
        unit_assert(a[i] == i, "quicksort failed");
    }

    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(test_quicksort_and_quickselect);
    return NULL;
}

RUN_TESTS(all_tests)
