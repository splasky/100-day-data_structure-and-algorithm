#include "../include/stack.h"
#include "unit.h"
#include <stdio.h>

char* tests[] = { "test1 data", "test2 data", "test3 data" };
#define NUM_TESTS 3

TEST(test_stack)
{
    Stack* stack = New_Stack();
    int i = 0;
    for (i = 0; i < NUM_TESTS; i++)
    {
        Stack_push(stack, tests[i]);
    }

    for (i = NUM_TESTS - 1; i >= 0; i--)
    {
        char* val = Stack_pop(stack);
        printf("%s\n", val);
    }

    Stack_destory(stack);
    return 0;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(test_stack);
    return NULL;
}

RUN_TESTS(all_tests);
