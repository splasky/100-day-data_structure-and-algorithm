#include "../include/avltree.h"
#include "unit.h"

static int compare(const void* a, const void* b)
{
    if (*(int*)a > *(int*)b) {
        return 1;
    } else if (*(int*)a == *(int*)b) {
        return 0;
    }
    return -1;
}

static void print(const AVLTreeNode* root) { printf("%d\n", *(int*)root->data); }

TEST(test_avltree)
{
    const int n = 10;
    const int randRange = 1000;
    const int randTrials = 10000;
    AVLTreeNode* root = TREE_EMPTY;

    for (int i = 0; i < n; ++i) {
        int* data = malloc(sizeof(int));
        *data = i;
        int* key = malloc(sizeof(int));
        *key = i;

        unit_assert(!avltreeContains(root, &i), "Test not contains failed");
        avltreeInsert(&root, key, data, compare);
        unit_assert(avltreeContains(root, &i), "Test not contains failed");
        avltreeSanityCheck(root);
        avltreePrint(root, print);
        printf("---\n");
    }

    /* delete everything */
    for (int i = 0; i < n; ++i) {
        unit_assert(avltreeContains(root, &i), "Test contains failed");
        avltreeDelete(&root, &i);
        unit_assert(!avltreeContains(root, &i), "Test delete failed");
        avltreeSanityCheck(root);
        avltreePrint(root, print);
        printf("---\n");
    }

    avltreeSanityCheck(root);
    avltreeDestroy(&root);

    srand(1);
    for (int i = 0; i < randTrials; i++) {
        int* data = malloc(sizeof(int));
        *data = rand() % randRange;
        int* key = malloc(sizeof(int));
        *key = rand() % randRange;

        avltreeInsert(&root, key, data, compare);
        avltreeDelete(&root, key);
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
