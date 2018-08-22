#ifndef SORT_H_YH2ZDUCO
#define SORT_H_YH2ZDUCO

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../include/dbg.h"

#define IS_EQUAL 0
#define IS_MORE 1
#define IS_LESS -1

typedef int (*Compare)(const void* a, const void* b);

static inline void generic_swap(void* va, void* vb, size_t size)
{
    uint8_t temp = *(uint8_t*)va;
    uint8_t* a = (uint8_t*)va;
    uint8_t* b = (uint8_t*)vb;
    while (size--)
    {
        temp = a[size];
        a[size] = b[size];
        b[size] = temp;
    }
}

static void print_array(void* array,
                        const size_t len,
                        const size_t object_size,
                        void (*print)(void*))
{
    for (size_t i = 0; i < len; ++i)
    {
        print((uint8_t*)array + i * object_size);
    }
    printf("\n");
}

static void shuffle(void* array,
                    const size_t len,
                    const size_t object_size)
{
    for (size_t i = len - 1; i > 0; i--)
    {
        uint32_t r = rand() % i;
        generic_swap((uint8_t*)array + i * object_size, (uint8_t*)array + r *
                     object_size, object_size);
    }
}

static void bubble_sort(void* array,
                        const size_t len,
                        const size_t object_size,
                        Compare compare)
{
    uint8_t* base = (uint8_t*)array;
    for (int i = len - 1; i > 0; --i)
    {
        for (int j = 0; j < i; ++j)
        {
            void* va = base + j * object_size;
            void* vb = base + (j + 1) * object_size;
            if (compare(va, vb) > IS_EQUAL)
            {
                generic_swap(va, vb, object_size);
            }
        }
    }
}

static void insert_sort(void* array,
                        const size_t len,
                        const size_t object_size,
                        Compare compare)
{
    for (size_t i = 1; i < len; i++)
    {
        uint8_t* base  = (uint8_t*)array;
        int j = 0;
        for (j = i;  j > 0; j--)
        {
            uint8_t* ele1 = (base + j * object_size);
            uint8_t* ele2 = (base + (j - 1) * object_size);
            if (compare(ele1, ele2 ) < IS_EQUAL)
            {
                generic_swap(ele1, ele2, object_size);
            }
        }
    }
}

static void select_sort(void* array,
                        const size_t len,
                        const size_t object_size,
                        Compare compare)
{
    uint32_t min = 0;
    uint8_t* base = (uint8_t*)array;
    for (uint32_t i = 0; i < len; ++i)
    {
        min = i;
        for (uint32_t j = i + 1; j < len; ++j)
        {
            uint8_t* ele1 = (base + j * object_size);
            uint8_t* ele2 = (base + min * object_size);
            if (compare(ele1, ele2) == IS_LESS)
            {
                min = j;
            }
        }

        uint8_t* ele1 = (base + i * object_size);
        uint8_t* ele2 = (base + min * object_size);
        if (i != min) generic_swap(ele1, ele2, object_size);
    }
}

static void shell_sort(void* array,
                       const size_t len,
                       const size_t object_size,
                       Compare compare)
{
    for (int gap = len / 2; gap > 0; gap /= 2)
    {
        insert_sort(array, len, object_size, compare);
    }
}

static inline int partition(void* array,
                            const size_t object_size,
                            const int front,
                            const int end,
                            Compare compare)
{
    uint8_t* base = (uint8_t*)array;
    const void* pivot = base + end * object_size;
    int i = front - 1;
    for (int j = front; j < end; ++j)
    {
        void* arr_j = base + j * object_size;
        if (compare(arr_j, pivot) == IS_LESS  )
        {
            ++i;
            void* arr_i = base + i * object_size;
            generic_swap(arr_i, arr_j, object_size);
        }
    }

    ++i;
    void* arr_i = base + i * object_size;
    void* arr_end = base + end * object_size;
    generic_swap(arr_i, arr_end, object_size);
    return i;
}

static void quick_sort_inline(void* array,
                              const size_t object_size,
                              const int front,
                              const int end,
                              Compare compare)
{
    if (front < end)
    {
        const int pivot = partition(array, object_size, front, end, compare);
        quick_sort_inline(array, object_size, front, pivot - 1, compare);
        quick_sort_inline(array, object_size, pivot + 1, end, compare);
    }
}

static void quick_sort(void* array,
                       const size_t len,
                       const size_t object_size,
                       int (*compare)(const void* a, const void* b))
{
    quick_sort_inline(array, object_size, 0, len - 1, compare);
}

static void max_heapify(void* array,
                        const size_t object_size,
                        Compare compare,
                        const int start,
                        const int end)
{
    uint8_t* base = (uint8_t*)array;
    int dad = start;
    int son =
        (dad << 1) + 1;

    while (son <= end)
    {
        void* arr_son = base + son * object_size;
        void* arr_next_son = base + (son + 1) * object_size;
        if (son + 1 <= end && compare(arr_son, arr_next_son) == IS_LESS) son++;
        void* arr_dad = base + dad * object_size;
        arr_son = base + son * object_size;
        if (compare(arr_dad, arr_son) == IS_MORE)
        {
            return;
        }
        else
        {
            generic_swap(arr_dad, arr_son, object_size);
            dad = son;
            son = (dad << 1) + 1;
        }
    }
}

static void heap_sort(void* array,
                      const size_t len,
                      const size_t object_size,
                      Compare compare)
{
    uint8_t* base = (uint8_t*)array;
    /* change array to heap  */
    for (int i = len / 2 - 1; i >= 0; i--)
    {
        max_heapify(array, object_size, compare, i, len - 1);
    }

    /* change root with last leaf */
    for (int i = len - 1; i > 0; i--)
    {
        void* arr_0 = base;
        void* arr_i = base + i * object_size;
        generic_swap(arr_0, arr_i, object_size);
        max_heapify(array, object_size, compare, 0, i - 1);
    }
}

/* static void merge(const int n1,
                  const void* a1,
                  const int n2,
                  const void* a2,
                  const size_t object_size,
                  Compare compare,
                  const void* out)
{
    int i1 = 0,
        i2 = 0, iout = 0;

    while (i1 < n1 || i2 < n2)
    {
        if (i2 >= n2 || ((i1 < n1) && (compare((uint8_t*)a1 + i1 * object_size,
                                               (uint8_t*)a2 + i2 * object_size) == IS_LESS)))
        {
            [>a1[i1] exists and is smaller<]
            memmove((uint8_t*)out + iout * object_size,
                    (uint8_t*)a1 + i1 * object_size,
                    object_size);
            ++i1;
        }
        else
        {
            memmove((uint8_t*)out + iout * object_size,
                    (uint8_t*)a2 + i2 * object_size,
                    object_size);
            ++i2;
        }
        ++iout;
    }
}

static void merge_sort_inline(void* array,
                              const size_t n,
                              const size_t object_size,
                              Compare compare,
                              void* out)
{

    if (n < 2)
    {
        [>0 or 1 elements is already sorted<]
        memcpy(out, array, sizeof(object_size) * n);
    }
    else
    {
        void* a1 = malloc(sizeof(object_size) * (n / 2));
        void* a2 = malloc(sizeof(object_size) * (n - n / 2));

        merge_sort_inline((uint8_t*)array, n / 2, object_size, compare, a1);
        merge_sort_inline((uint8_t*)array + n / 2, n - n / 2, object_size, compare, a2);

        [>merge results<]
        merge(n / 2, a1, n - n / 2, a2, object_size, compare, out);
        free(a1);
        free(a2);
    }
}

static void merge_sort(void* array,
                       const size_t len,
                       const size_t object_size,
                       Compare compare)
{
    void* out = calloc(len, sizeof(object_size));
    merge_sort_inline(array, len, object_size, compare, out);
    memcpy(array, out, sizeof(object_size)*len);
    free(out);
}
 */

#endif //end of include guard: SORT_H_YH2ZDUCO
