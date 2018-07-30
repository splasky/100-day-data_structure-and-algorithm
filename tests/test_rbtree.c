#include "../include/rbtree.h"
#include "unit.h"

static int compare(const void* a, const void* b)
{
    if (*(int*)a > *(int*)b)
    {
        return 1;
    }
    else if (*(int*)a == *(int*)b)
    {
        return 0;
    }
    return -1;
}

static void print(const RbNode* root)
{
    printf("%d(%d)\n", *(int*)root->data, root->color);
}

TEST(test_RbTree)
{
    const int n = 100;
    const int randRange = 1000;
    const int randTrials = 10000;
    RbTree* rbtree = New_RbTree(compare);

    for (int i = 0; i < n; ++i)
    {
        int* data = malloc(sizeof(int));
        *data = i;
        int* key = malloc(sizeof(int));
        *key = i;

        unit_assert(!RbTree_get(rbtree, &i), "Test not contains failed");
        RbTree_insert(rbtree, key, data);
        unit_assert(RbTree_get(rbtree, &i), "Test not contains failed");
        RbTree_printTree(rbtree, print);
        printf("---\n");
    }

    /* delete everything */
    for (int i = 0; i < n; ++i)
    {
        unit_assert(RbTree_get(rbtree, &i), "Test contains failed");
        RbTree_delete(rbtree, &i);
        unit_assert(!RbTree_get(rbtree, &i), "Test delete failed");
        RbTree_printTree(rbtree, print);
        printf("---\n");
    }


    srand(1);
    for (int i = 0; i < randTrials; i++)
    {
        int* data = malloc(sizeof(int));
        *data = rand() % randRange;
        int* key = malloc(sizeof(int));
        *key = rand() % randRange;

        RbTree_insert(rbtree, key, data);
        RbTree_delete(rbtree, key);
    }

    RbTree_printTree(rbtree, print);
    RbTree_destroy(rbtree);

    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(test_RbTree);
    return NULL;
}

RUN_TESTS(all_tests);
