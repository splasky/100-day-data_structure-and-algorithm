#include "../include/dbg.h"
#include "../include/trie.h"
#include "unit.h"

char* test_string[] = { "abc", "bca", "acb", "bcd", "cbd", "abcd", "abc" };

TEST(test_trie)
{
    Trie* t = EMPTY_TRIE;

    char** ptr = test_string;

    for (int i = 0; i < 7; ++i) {
        printf("%s\n", *ptr);
        t = trie_insert(t, *ptr);
        unit_assert(trie_contains(t, *ptr), "Test insert failed");
        ptr++;
    }

    printf("====\n");
    trie_print(t);
    trie_destroy(t);
    return NULL;
}

TEST(all_tests)
{

    unit_suite_start();
    unit_run_test(test_trie);
    return NULL;
}

RUN_TESTS(all_tests);
