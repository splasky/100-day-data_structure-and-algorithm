#include "../include/skiplist.h"
#include "unit.h"

TEST(test_skiplist)
{

    Skiplist* splist = skiplist_create();
    unit_assert(splist, "Create skip list failed");

    for (int i = 0; i < 10; i += 2) {
        skiplistInsert(splist, i);
    }

    for (int i = 0; i < 10; i += 2) {
        unit_assert(skiplist_search(splist, i) == i, "Skiplist Insert failed");
    }

    for (int i = 0; i < 10; i += 3) {
        skiplistDelete(splist, i);
        unit_assert(skiplist_search(splist, i) < i, "Skiplist delete failed");
    }

    skiplist_destroy(splist);
    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(test_skiplist);
    return NULL;
}

RUN_TESTS(all_tests);
