#include "../include/circular_queue.h"
#include "unit.h"
#include <stdint.h>
#include <stdio.h>

#define MAX 10

Circular_Queue* cqueue = NULL;

TEST(create_new_circular_queue)
{
    cqueue = New_Circular_Queue(MAX);
    unit_assert(cqueue != NULL, "Create new circular queue failed");
    return NULL;
}

TEST(circular_isEmpty)
{
    unit_assert(is_Circular_Queue_Empty(cqueue), "test is_empty failed");
    for (int i = 0; i < MAX; i++) {
        int* value = malloc(sizeof(int));
        *value = i;
        Circular_Queue_enqueue(cqueue, value);
    }
    unit_assert(is_Circular_Queue_Full(cqueue), "test is_full faild");
    return NULL;
}

TEST(dequeue)
{
    int* x = Circular_Queue_dequeue(cqueue);
    unit_assert(*x == 0, "test dequeue failed");
    free(x);
    return NULL;
}

TEST(clear_and_destory)
{
    Circular_Queue_destory_and_clean(cqueue);
    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(create_new_circular_queue);
    unit_run_test(circular_isEmpty);
    unit_run_test(dequeue);
    unit_run_test(clear_and_destory);

    return NULL;
}

RUN_TESTS(all_tests);
