#include "../include/bstree.h"
#include "unit.h"

BSTree* bstree = NULL;

TEST(create_bstree)
{
    bstree = New_BSTree(NULL);
    unit_assert(bstree != NULL, "create btree failed.");
    return NULL;
}

TEST(destory)
{
    BSTree_destory(bstree);
    return NULL;
}

static inline int print_bstree_node(BSTreeNode* node)
{
    printf("%d %d\n", *(int*)node->key, *(int*)node->data);
    return 0;
}

TEST(set_get_traverse)
{
    int i[] = { 1, 3, 4, 6, 10, 3, 2, 53, 64, 32, 56, 12, 0, 80, 99 };
    int size = sizeof(i) / sizeof(i[0]);
    for (int j = 0; j < size; j++) {
        int* p = malloc(sizeof(int));
        *p = i[j];
        BSTree_set(bstree, p, p);
    }

    BSTree_traverse(bstree, print_bstree_node);

    int key = 3;
    int* ptr = &key;
    int* p = BSTree_get(bstree, ptr);
    debug("%d\n", *p);
    unit_assert(*p == 3, "get key from bstree failed");

    return NULL;
}

TEST(delete)
{
    int key = 4;
    int* ptr = &key;
    int* p = BSTree_delete(bstree, ptr);
    debug("%d\n", *p);
    unit_assert(*p == 4, "delete key failed");
    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(create_bstree);
    unit_run_test(set_get_traverse);
    unit_run_test(delete);
    unit_run_test(destory);

    return NULL;
}

RUN_TESTS(all_tests)
