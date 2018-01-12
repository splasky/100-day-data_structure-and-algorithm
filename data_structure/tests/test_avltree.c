#include "../include/avltree.h"
#include "unit.h"

TEST(test_avltree)
{
    const int n = 10;
    const int randRange = 1000;
    const int randTrials = 10000;
    struct avltree* root = TREE_EMPTY;

    for (int i = 0; i < n; ++i) {
        unit_assert(!avltreeContains(root, i), "Test not contains failed");
        avltreeInsert(&root, i);
        unit_assert(avltreeContains(root, i), "Test not contains failed");
        avltreeSanityCheck(root);
        avltreePrint(root);
        printf("---\n");
    }

    /* check ranks */
    for (int i = 0; i < n; ++i) {
        unit_assert(avltreeRank(root, i) == (size_t)i, "Test rank failed");
        unit_assert(avltreeUnrank(root, i) == i, "Test unrank failed");
    }

    /* delete everything */
    for (int i = 0; i < n; ++i) {
        unit_assert(avltreeContains(root, i), "Test contains failed");
        avltreeDelete(&root, i);
        unit_assert(!avltreeContains(root, i), "Test delete failed");
        avltreeSanityCheck(root);
        avltreePrint(root);
        printf("---\n");
    }

    avltreeSanityCheck(root);
    avltreeDestroy(&root);

    srand(1);
    for (int i = 0; i < randTrials; i++) {
        avltreeInsert(&root, rand() % randRange);
        avltreeDelete(&root, rand() % randRange);
    }

    avltreeSanityCheck(root);
    avltreeDestroy(&root);

    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(test_avltree);
    return NULL;
}

RUN_TESTS(all_tests);
