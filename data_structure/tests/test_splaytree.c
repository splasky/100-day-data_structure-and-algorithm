#include "../include/splaytree.h"
#include "unit.h"

TEST(test_splaytree)
{
    int i;
    const int n = 10;
    struct splaytree* root = TREE_EMPTY;

    for (i = 0; i < n; i++) {
        unit_assert(!splaytreeContains(&root, i), "Test splaytreeContains failed");
        splaytreeInsert(&root, i);
        unit_assert(splaytreeContains(&root, i), "Test splaytreeContains failed");
        splaytreePrint(root);
        puts("===");
    }

    /* now delete everything */
    for (i = 0; i < n; i++) {
        unit_assert(splaytreeContains(&root, i), "Test splaytreeContains failed");
        splaytreeDelete(&root, i);
        unit_assert(!splaytreeContains(&root, i), "Test splaytreeContains failed");
        splaytreePrint(root);
        puts("===");
    }

    splaytreeDestroy(&root);
    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(test_splaytree);
    return NULL;
}

RUN_TESTS(all_tests);
