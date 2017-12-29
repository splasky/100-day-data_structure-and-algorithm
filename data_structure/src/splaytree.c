#include "../include/splaytree.h"
#include "../include/dbg.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

void splaytreeDestroy(struct splaytree** root)
{
    while (*root) {
        splaytreeDelete(root, (*root)->key);
    }
}

static void treeRotate(struct splaytree** root, int direction)
{
    struct splaytree* x;
    struct splaytree* y;
    struct splaytree* b;

    /*
    *      y           x
    *     / \         / \
    *    x   C  <=>  A   y
    *   / \             / \
    *  A   B           B   C
    */

    y = *root;
    x = y->child[direction];
    b = x->child[!direction];

    *root = x;
    x->child[!direction] = y;
    y->child[direction] = b;
}

static inline void treeLink(
    struct splaytree*** hook, int direction, struct splaytree* node)
{
    *hook[direction] = node;
    node->child[!direction] = 0;
    hook[direction] = &node->child[!direction]; /* TODO:is this should be use before? */
}

/* splay last element on path to target to root */
static void treeSplay(struct splaytree** root, int target)
{
    struct splaytree* t;
    struct splaytree* child;
    struct splaytree* grandchild;
    struct splaytree* top[TREE_NUM_CHILDREN]; /* accumulator trees that will become
                                                 subtrees of new root */
    struct splaytree**
        hook[TREE_NUM_CHILDREN]; /* where to link new elements into accumulator trees */
    int d;
    int dChild;      /* direction of child */
    int dGrandchild; /* direction of grandchild */

    /* we don't need to keep following this pointer, we'll just fix it at the end */
    t = *root;

    /* don't do anything to an empty tree */
    if (t == 0) {
        return;
    }

    /* ok, tree is not empty, start chopping it up */
    for (d = 0; d < TREE_NUM_CHILDREN; d++) {
        top[d] = 0;
        hook[d] = &top[d];
    }

    /* keep going until we hit the key or we would hit a null pointer in the child */
    while (t->key != target && (child = t->child[dChild = t->key < target]) != 0) {
        /* child is not null */
        grandchild = child->child[dGrandchild = child->key < target];

#ifdef DEBUG_SPLAY
        treePrint(top[0]);
        puts("---");
        treePrint(t);
        puts("---");
        treePrint(top[1]);
        puts("===");
#endif

        if (grandchild == 0 || child->key == target) {
            /* zig case; paste root into opposite-side hook */
            treeLink(hook, !dChild, t);
            t = child;
            /* we can break because we know we will hit child == 0 next */
            break;
        } else if (dChild == dGrandchild) {
            /* zig-zig case */
            /* rotate and then hook up child */
            /* grandChild becomes new root */
            treeRotate(&t, dChild);
            treeLink(hook, !dChild, child);
            t = grandchild;
        } else {
            /* zig-zag case */
            /* root goes to !dChild, child goes to dChild, grandchild goes to root */
            treeLink(hook, !dChild, t);
            treeLink(hook, dChild, child);
            t = grandchild;
        }
    }

    /* now reassemble the tree */
    /* t's children go in hooks, top nodes become t's new children */
    for (d = 0; d < TREE_NUM_CHILDREN; d++) {
        *hook[d] = t->child[d];
        t->child[d] = top[d];
    }

    /* and put t back in *root */
    *root = t;
}

/* return 1 if target is in tree, 0 otherwise */
int splaytreeContains(struct splaytree** root, int target)
{
    treeSplay(root, target);
    return *root != 0 && (*root)->key == target;
}

/* insert an element into a tree pointed to by root */
void splaytreeInsert(struct splaytree** root, int new_element)
{
    struct splaytree* e;
    struct splaytree* t;
    int d; /* which side of e to put old root on */

    treeSplay(root, new_element);

    t = *root;

    /* skip if already present */
    if (t && t->key == new_element) {
        return;
    }

    /* otherwise split the tree */
    e = malloc(sizeof(*e));
    check_mem(e);

    e->key = new_element;

    if (t == 0) {
        e->child[LEFT] = e->child[RIGHT] = 0;
    } else {
        /* split tree and put e on top */
        /* we know t is closest to e, so we don't have to move anything else */
        d = (*root)->key > new_element;
        e->child[d] = t;
        e->child[!d] = t->child[!d];
        t->child[!d] = 0;
    }

    /* either way we stuff e in *root */
    *root = e;
error:
    log_err("malloc temp memory failed");
}

/* delete target from the tree */
/* has no effect if target is not in tree */
void splaytreeDelete(struct splaytree** root, int key)
{
    struct splaytree* left;
    struct splaytree* right;

    treeSplay(root, key);

    if (*root && (*root)->key == key) {
        /* save pointers to kids */
        left = (*root)->child[LEFT];
        right = (*root)->child[RIGHT];

        /* free the old root */
        free(*root);

        /* if left is empty, just return right */
        if (left == 0) {
            *root = right;
        } else {
            /* first splay max element in left to top */
            treeSplay(&left, INT_MAX);

            /* now paste in right subtree */
            left->child[RIGHT] = right;

            /* return left */
            *root = left;
        }
    }
}

/* how far to indent each level of the tree */
#define INDENTATION_LEVEL (2)

/* print contents of a tree, indented by depth */
static void treePrintIndented(const struct splaytree* root, int depth)
{
    int i;

    if (root != 0) {
        treePrintIndented(root->child[LEFT], depth + 1);

        for (i = 0; i < INDENTATION_LEVEL * depth; i++) {
            putchar(' ');
        }
        printf("%d (%p)\n", root->key, (void*)root);

        treePrintIndented(root->child[RIGHT], depth + 1);
    }
}

/* print the contents of a tree */
void splaytreePrint(const struct splaytree* root) { treePrintIndented(root, 0); }
