#include "../include/dbg.h"
#include "../include/tst.h"
#include "unit.h"

char* test_string[] = { "abc", "bca", "acb", "bcd", "cbd", "abcd", "abc" };

TEST(test_tst)
{
    TST* t = EMPTY_TST;

    char** ptr = test_string;

    for (int i = 0; i < 7; ++i) {
        printf("%s\n", *ptr);
        t = tst_insert(t, *ptr);
        unit_assert(tst_contains(t, *ptr), "Test insert failed");
        ptr++;
    }

    printf("====\n");
    tst_destroy(t);
    return NULL;
}

TEST(all_tests)
{

    unit_suite_start();
    unit_run_test(test_tst);
    return NULL;
}

RUN_TESTS(all_tests);
