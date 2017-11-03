#include "../include/stack.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

int isOperator(char* c)
{
    switch (*c) {
    case '+':
    case '-':
    case '*':
    case '/':
    case ')':
    case '(':
        return 1;
    default:
        return 0;
    }
}

int priority(char* c)
{
    switch (*c) {
    case '+':
    case '-':
        return 2;
    case '*':
    case '/':
        return 3;
    case '(':
        return 1;
    default:
        return 0;
    }
}

void postfix(char* infix, size_t size)
{
    char* curr = infix;

    Stack* op_stack = New_Stack();
    Stack* postfix_stack = New_Stack();

    for (int pc = 0; pc < size; ++pc) {
        if (*(curr + pc) == '(') {
            Stack_push(op_stack, (char*)(curr + pc));
        } else if (*(curr + pc) == ')') {
            char* top_op = Stack_pop(op_stack);
            while (*top_op != '(') {
                Stack_push(postfix_stack, top_op);
                top_op = Stack_pop(op_stack);
            }
        } else if (*(curr + pc) > '0' && *(curr + pc) < '9') {
            Stack_push(postfix_stack, (char*)(curr + pc));
        } else {
            while (Stack_count(op_stack) > 0
                && (priority((char*)Stack_peek(op_stack)->value) > priority(curr + pc))) {
                Stack_push(postfix_stack, (char*)Stack_pop(op_stack));
            }
            Stack_push(op_stack, (char*)(curr + pc));
        }
    }

    while (Stack_count(op_stack) > 0) {
        Stack_push(postfix_stack, (char*)Stack_pop(op_stack));
    }

    /* print */
    printf("inside stack:");
    for (int i = 0; i < size; ++i) {
        char* p = Stack_pop(postfix_stack);
        if (p == NULL) {
            break;
        }
        printf("%c", *p);
    }
    printf("\n");

    Stack_destory(op_stack);
    Stack_destory(postfix_stack);
}

int main(void)
{
    char* str = "(3+5)*8-6*(5-3/2+3*(4+5))";
    printf("Running test_postfix:\n");
    printf("origin:%s\n", str);
    postfix(str, strlen(str));
    return 0;
}
