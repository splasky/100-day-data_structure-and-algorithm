#include "../include/avltree.h"
#include "../include/dbg.h"
#include <stdio.h>

static int treeComputeHeight(const AVLTreeNode* root)
{

    if (root == TREE_EMPTY)
    {
        return TREE_EMPTY_HEIGHT;
    }
    int maxChildHeight;
    maxChildHeight = TREE_EMPTY_HEIGHT;

    for (int i = 0; i < TREE_NUM_CHILDREN; ++i)
    {
        int childHeight = avltreeHeight(root->child[i]);
        if (childHeight > maxChildHeight)
        {
            maxChildHeight = childHeight;
        }
    }

    return maxChildHeight + 1;
}

static int treeComputeSize(const AVLTreeNode* root)
{
    int size;

    if (root == TREE_EMPTY)
    {
        return 0;
    }
    size = 1;

    for (int i = 0; i < TREE_NUM_CHILDREN; ++i)
    {
        size += avltreeSize(root->child[i]);
    }

    return size;
}

static void treeAggregateFix(AVLTreeNode* root)
{
    if (root)
    {
        root->height = treeComputeHeight(root);
        root->size = treeComputeSize(root);
    }
}

static void treeRotate(AVLTreeNode** root, int direction)
{
    AVLTreeNode* x;
    AVLTreeNode* y;
    AVLTreeNode* b;

    y = *root;
    x = y->child[direction];
    b = x->child[!direction];

    *root = x;
    x->child[!direction] = y;
    y->child[direction] = b;

    treeAggregateFix(y);
    treeAggregateFix(x);
}

static void treeRebalance(AVLTreeNode** root)
{
    if (*root)
    {
        for (int direction = 0; direction < TREE_NUM_CHILDREN; ++direction)
        {
            if (avltreeHeight((*root)->child[direction])
                    >= avltreeHeight((*root)->child[!direction]) + 2)
            {

                if (avltreeHeight((*root)->child[direction]->child[!direction])
                        > avltreeHeight((*root)->child[direction]->child[direction]))
                {
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

static inline AVLTreeNode* Create_AVLTreeNode(
    void* key, void* data, AVLTree_compare compare)
{
    AVLTreeNode* node = malloc(sizeof(AVLTreeNode));
    node->compare = compare;
    node->data = data;
    node->key = key;
    node->height = 0;
    node->size = 1;
    node->child[LEFT] = node->child[RIGHT] = TREE_EMPTY;
    return node;
}

void avltreeDestroy(AVLTreeNode** root)
{
    if (*root)
    {
        for (int i = 0; i < TREE_NUM_CHILDREN; ++i)
        {
            avltreeDestroy(&(*root)->child[i]);
        }
        free(*root);
        *root = TREE_EMPTY;
    }
}

void avltreeInsert(AVLTreeNode** root, void* key, void* data, AVLTree_compare compare)
{

    if (*root == TREE_EMPTY)
    {
        AVLTreeNode* e = Create_AVLTreeNode(key, data, compare);
        check_mem(e);
        *root = e;
    }
    else if ((*root)->compare((*root)->key, key) == 0)
    {
        /* element exists,do nothing */
        return;
    }
    else
    {
        avltreeInsert(
            &(*root)->child[(*root)->compare((*root)->key, key) < 0], key, data, compare);
    }

    treeAggregateFix(*root);
    treeRebalance(root);
error:
    return;
}

bool avltreeContains(const AVLTreeNode* root, const void* key)
{
    if (!root)
        return NULL;
    while (root != TREE_EMPTY)
    {
        if (root->compare(root->key, key) == 0)
        {
            return true;
        }
        root = root->child[root->compare(root->key, key) < 0];
    }
    return false;
}

void* avltreeFind(const AVLTreeNode* root, const void* key)
{
    check(root, "root is empty");
    while (root != TREE_EMPTY)
    {
        if (root->compare(root->key, key) == 0)
        {
            return root->data;
        }
        root = root->child[root->compare(root->key, key) < 0];
    }
error:
    return NULL;
}

void* avltreeDeleteMin(AVLTreeNode** root)
{
    AVLTreeNode* toFree;
    void* retval;
    check(*root, "root is empty");

    if ((*root)->child[LEFT])
    {
        retval = avltreeDeleteMin(&(*root)->child[LEFT]);
    }
    else
    {
        toFree = *root;
        retval = toFree->data;
        *root = toFree->child[RIGHT];
        free(toFree);
    }

    treeAggregateFix(*root);
    treeRebalance(root);

    return retval;
error:
    return 0;
}

void avltreeDelete(AVLTreeNode** root, void* target)
{
    AVLTreeNode* toFree;

    if (*root)
    {
        if ((*root)->compare((*root)->key, target) == 0)
        {
            if ((*root)->child[RIGHT])
            {
                (*root)->key = avltreeDeleteMin(&(*root)->child[RIGHT]);
            }
            else
            {
                toFree = *root;
                *root = toFree->child[LEFT];
                free(toFree);
            }
        }
        else
        {
            avltreeDelete(
                &(*root)->child[(*root)->compare((*root)->key, target) < 0 ], target);
        }

        treeAggregateFix(*root);
        treeRebalance(root);
    }
}

int avltreeHeight(const AVLTreeNode* root)
{
    if (root == TREE_EMPTY)
    {
        return TREE_EMPTY_HEIGHT;
    }
    else
    {
        return root->height;
    }
}

size_t avltreeSize(const AVLTreeNode* root)
{
    if (root == TREE_EMPTY)
    {
        return 0;
    }
    else
    {
        return root->size;
    }
}

#define INDENTATION_LEVEL (2)

static void treePrintIndented(const AVLTreeNode* root, int depth, AVLTree_print print)
{
    if (root)
    {
        /* print right subtree */
        treePrintIndented(root->child[RIGHT], depth + 1, print);
        for (int i = 0; i < INDENTATION_LEVEL * depth; ++i)
        {
            printf(" ");
        }
        print(root);
        /* print left subtree */
        treePrintIndented(root->child[LEFT], depth + 1, print);
    }
}

void avltreePrint(const AVLTreeNode* root, AVLTree_print print)
{
    treePrintIndented(root, 0, print);
}

void avltreeSanityCheck(const AVLTreeNode* root)
{

    if (root)
    {
        check(root->height == treeComputeHeight(root), "Height not correct");
        check(root->size == (size_t)treeComputeSize(root), "Size not correct");
        check(
            abs(avltreeHeight(root->child[LEFT]) - avltreeHeight(root->child[RIGHT])) < 2,
            "Not balance");

        for (int i = 0; i < TREE_NUM_CHILDREN; ++i)
        {
            avltreeSanityCheck(root->child[i]);
        }
    }

    return;
error:
    log_info("Avltree check failed");
}
