#include "../include/dbg.h"
#include "unit.h"
#include <stdio.h>
#include <stdlib.h>

#define N 20

void merge(int n1, const int a1[], int n2, const int a2[], int out[])
{
    int i1, i2, iout;
    i1 = i2 = iout = 0;

    while (i1 < n1 || i2 < n2) {
        if (i2 >= n2 || ((i1 < n1) && (a1[i1] < a2[i2]))) {
            /* a1[i1] exists and is smaller */
            out[iout++] = a1[i1++];
        } else {
            out[iout++] = a2[i2++];
        }
    }
}

void mergeSort(int n, const int a[], int out[])
{

    int *a1, *a2;

    if (n < 2) {
        /* 0 or 1 elements is already sorted */
        memcpy(out, a, sizeof(int) * n);
    } else {
        a1 = malloc(sizeof(int) * (n / 2));
        a2 = malloc(sizeof(int) * (n - n / 2));

        mergeSort(n / 2, a, a1);
        mergeSort(n - n / 2, a + n / 2, a2);

        /* merge results */
        merge(n / 2, a1, n - n / 2, a2, out);

        free(a1);
        free(a2);
    }
}

TEST(test_merge_sort)
{
    int a[N];
    int b[N];

    for (int i = 0; i < N; ++i) {
        a[i] = N - i - 1;
    }

    /* print a[] */
    for (int i = 0; i < N; ++i) {
        printf("%d\n", a[i]);
    }
    printf("\n");

    mergeSort(N, a, b);

    for (int i = 0; i < N; ++i) {
        printf("%d\n", b[i]);
    }

    printf("\n");
    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(test_merge_sort);
    return NULL;
}

RUN_TESTS(all_tests);
