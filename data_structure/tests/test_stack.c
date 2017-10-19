#include "include/stack.h"
#include <stdio.h>

char* tests[] = { "test1 data", "test2 data", "test3 data" };
#define NUM_TESTS 3

int main(int argc, char* argv[])
{
    Stack* stack = New_Stack();
    int i = 0;
    for (i = 0; i < NUM_TESTS; i++) {
        Stack_push(stack, tests[i]);
    }

    Stack_FOREACH(stack, cur) { printf("%s\n", (char*)cur->value); }

    for (i = NUM_TESTS - 1; i >= 0; i--) {
        char* val = Stack_pop(stack);
        printf("%s\n", val);
    }

    Stack_destory(stack);
    return 0;
}
