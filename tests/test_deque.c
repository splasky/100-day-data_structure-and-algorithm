#include "../include/deque.h"
#include "unit.h"
#include <assert.h>
#include <stdio.h>

Deque* dq = NULL;
TEST(allo_new_deque)
{
    dq = New_Deque();
    unit_assert(dq != NULL, "create deque failed");
    return NULL;
}

TEST(deque_push_back)
{

    for (int i = 1; i <= 256; i += i) {
        int* p = malloc(sizeof(int));
        *p = i;
        Deque_push_back(dq, p);
    }

    while (Deque_count(dq) > 0) {
        int* c = (int*)Deque_pop_front(dq);
        printf("%d\n", *c);
        free(c);
        c = NULL;
    }

    unit_assert(Deque_count(dq) == 0, "test push pop failed");
    return NULL;
}

TEST(destory_deque)
{
    Deque_clear_destory(dq);
    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(allo_new_deque);
    unit_run_test(deque_push_back);
    unit_run_test(destory_deque);
    return NULL;
}

RUN_TESTS(all_tests)
