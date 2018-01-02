#include "../include/avltree.h"
#include "../include/dbg.h"
#include <stdio.h>

static int treeComputeHeight(const struct avltree* root)
{
    int childHeight;
    int maxChildHeight;

    if (root == 0) {
        return TREE_EMPTY_HEIGHT;
    } else {
        maxChildHeight = TREE_EMPTY_HEIGHT;

        for (int i = 0; i < TREE_NUM_CHILDREN; ++i) {
            childHeight = avltreeHeight(root->child[i]);
            if (childHeight > maxChildHeight) {
                maxChildHeight = childHeight;
            }
        }

        return maxChildHeight + 1;
    }
}

static int treeComputeSize(const struct avltree* root)
{
    int size;

    if (root == 0) {
        return 0;
    } else {
        size = 1;

        for (int i = 0; i < TREE_NUM_CHILDREN; ++i) {
            size += avltreeSize(root->child[i]);
        }

        return size;
    }
}

static void treeAggregateFix(struct avltree* root)
{
    if (root) {
        root->height = treeComputeHeight(root);
        root->size = treeComputeSize(root);
    }
}

static void treeRotate(struct avltree** root, int direction)
{
    struct avltree* x;
    struct avltree* y;
    struct avltree* b;

    y = *root;
    x = y->child[direction];
    b = x->child[!direction];

    *root = x;
    x->child[!direction] = y;
    y->child[direction] = b;

    treeAggregateFix(y);
    treeAggregateFix(x);
}

static void treeRebalance(struct avltree** root)
{
    if (*root) {
        for (int direction = 0; direction < TREE_NUM_CHILDREN; ++direction) {
            if (avltreeHeight((*root)->child[direction])
                >= avltreeHeight((*root)->child[!direction]) + 2) {

                if (avltreeHeight((*root)->child[direction]->child[!direction])
                    > avltreeHeight((*root)->child[direction]->child[direction])) {
                    treeRotate(&(*root)->child[direction], !direction);
                }

                treeRotate(root, direction);
                break;
            }
        }
        check((avltreeHeight((*root)->child[LEFT]) - avltreeHeight((*root)->child[RIGHT]))
                <= 1,
            "tree is not balance");
    }

error:
    return;
}

void avltreeDestroy(struct avltree** root)
{
    if (*root) {
        for (int i = 0; i < TREE_NUM_CHILDREN; ++i) {
            avltreeDestroy(&(*root)->child[i]);
        }
        free(*root);
        *root = TREE_EMPTY;
    }
}

void avltreeInsert(struct avltree** root, int new_element)
{

    struct avltree* e;
    if (*root == 0) {
        e = malloc(sizeof(*e));
        check_mem(e);

        e->key = new_element;
        e->child[LEFT] = e->child[RIGHT] = 0;

        *root = e;
    } else if ((*root)->key == new_element) {
        /* element exists,do nothing */
        return;
    } else {
        avltreeInsert(&(*root)->child[(*root)->key < new_element], new_element);
    }

    treeAggregateFix(*root);
    treeRebalance(root);
error:
    return;
}

int avltreeContains(const struct avltree* root, int target)
{
    while (root && root->key != target) {
        root = root->child[root->key < target];
    }

    return root != 0;
}

int avltreeDeleteMin(struct avltree** root)
{
    struct avltree* toFree;
    int retval;
    check(*root, "root is empty");

    if ((*root)->child[LEFT]) {
        retval = avltreeDeleteMin(&(*root)->child[LEFT]);
    } else {
        toFree = *root;
        retval = toFree->key;
        *root = toFree->child[RIGHT];
        free(toFree);
    }

    treeAggregateFix(*root);
    treeRebalance(root);

    return retval;
error:
    return 0;
}

void avltreeDelete(struct avltree** root, int target)
{
    struct avltree* toFree;

    if (*root) {
        if ((*root)->key == target) {
            if ((*root)->child[RIGHT]) {
                (*root)->key = avltreeDeleteMin(&(*root)->child[RIGHT]);
            } else {
                toFree = *root;
                *root = toFree->child[LEFT];
                free(toFree);
            }
        } else {
            avltreeDelete(&(*root)->child[(*root)->key < target], target);
        }

        treeAggregateFix(*root);
        treeRebalance(root);
    }
}

int avltreeHeight(const struct avltree* root)
{
    if (root == 0) {
        return TREE_EMPTY_HEIGHT;
    } else {
        return root->height;
    }
}

size_t avltreeSize(const struct avltree* root)
{
    if (root == 0) {
        return 0;
    } else {
        return root->size;
    }
}

#define INDENTATION_LEVEL (2)

static void treePrintIndented(const struct avltree* root, int depth)
{
    if (root) {
        treePrintIndented(root->child[LEFT], depth + 1);
        for (int i = 0; i < INDENTATION_LEVEL * depth; ++i) {
            printf(" ");
        }
        printf("%d Height: %d Size: %zu (%p)\n", root->key, root->height, root->size,
            (void*)root);

        treePrintIndented(root->child[RIGHT], depth + 1);
    }
}

void avltreePrint(const struct avltree* root) { treePrintIndented(root, 0); }

size_t avltreeRank(const struct avltree* root, int target)
{
    size_t rank = 0;

    while (root && root->key != target) {
        if (root->key < target) {
            rank += (1 + avltreeSize(root->child[LEFT]));
            root = root->child[RIGHT];
        } else {
            root = root->child[LEFT];
        }
    }

    return rank + avltreeSize(root->child[LEFT]);
}

int avltreeUnrank(const struct avltree* root, size_t rank)
{
    size_t leftSize;
    while (rank != (leftSize = avltreeSize(root->child[LEFT]))) {
        if (rank < leftSize) {
            root = root->child[LEFT];
        } else {
            root = root->child[RIGHT];
            rank -= (leftSize + 1);
        }
    }

    return root->key;
}

void avltreeSanityCheck(const struct avltree* root)
{

    if (root) {
        check(root->height == treeComputeHeight(root), "Height not correct");
        check(root->size == treeComputeSize(root), "Size not correct");
        check(
            abs(avltreeHeight(root->child[LEFT]) - avltreeHeight(root->child[RIGHT]) < 2),
            "Not balance");

        for (int i = 0; i < TREE_NUM_CHILDREN; ++i) {
            avltreeSanityCheck(root->child[i]);
        }
    }

    return;
error:
    log_info("Avltree check failed");
}
