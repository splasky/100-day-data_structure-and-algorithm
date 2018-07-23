#include "rbtree.h"
#include <stdlib.h>
#include "include/dbg.h"

static void balanceAfterInsert(RbNode* root, RbNode* newNode);
static bool uNodeIsBlackOrNull(RbNode* uNode);
static void leftRotate(RbNode** root, RbNode* pivot);
static void rightRotate(RbNode** root, RbNode* pivot);

RbTree* New_RbTree(RbTree_compare compare)
{
    RbTree* rbtree = malloc(sizeof(RbTree));
    rbtree->compare = compare;
    rbtree->height = 0;
    rbtree->root = NULL;
    return rbtree;
}

void RbTree_insert(RbTree* rbtree, void* id, void* data)
{
    RbNode_insert(rbtree->root, id, data, rbtree->compare);
}

void RbTree_delete(RbTree* rbtree, void* id)
{

}

void RbTree_update(RbTree* rbtree, void* id, void* data)
{
    RbNode* ret = RbTree_getNode(rbtree, id);
    free(ret->data);
    ret->data = data;
}

void* RbTree_getNode(RbTree* rbtree, void* id)
{
    RbNode* root = rbtree->root;
    while (root != NULL)
    {
        if (rbtree->compare(root->id, id) == 0)
        {
            return root;
        }
        root = rbtree->compare(root->id, id) < 0 ? root->left : root->right;
    }
    return NULL;
}

void* RbTree_get(RbTree* rbtree, void* id)
{
    RbNode* ret = RbTree_getNode(rbtree, id);
    if (!ret)
    {
        return NULL;
    }
    return ret->data;
}

void RbTree_destroy(RbTree* rbtree)
{
    free(rbtree);
}

RbNode* New_RbNode(void* id, void* data)
{
    RbNode* node = malloc(sizeof(RbNode));
    node->id = id;
    node->data = data;
    node->red = true;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

void RbNode_insert(RbNode* root, void* id, void* data, RbTree_compare compare)
{
    RbNode* newNode = New_RbNode(id, data);

    if (root == NULL)
    {
        root = newNode;
    }
    else
    {
        RbNode* current = root;
        RbNode* parent = NULL;

        while (true)
        {
            parent = current;
            if (compare(id, current->id) < 0)
            {
                current = current->left;
                if (current == NULL)
                {
                    parent->left = newNode;
                    newNode->parent = parent;
                    break;
                }
            }
            else
            {
                current = current->right;
                if (current == NULL)
                {
                    parent->right = newNode;
                    newNode->parent = parent;
                    break;

                }
            }
        }
    }
    balanceAfterInsert(root, newNode);
}

static void balanceAfterInsert(RbNode* root, RbNode* current)
{
    /* Case 1: if no parent */
    if (current->parent == NULL)
    {
        current->red = false;
        root = current;
    }
    else
    {
        /* Case 2: if current parent is black */
        if (!current->parent->red)
        {
            /* do nothing */
        }
        else if (current->parent->red)
        {
            /* gNode: grandfather */
            RbNode* gNode = current->parent->parent;
            RbNode* uNode = NULL;
            if (gNode != NULL)
            {
                /* uNode: uncle */
                uNode = (gNode->left == current->parent ? gNode->left : gNode->right);
            }
            /* Case 3: If node P and node U are all red, modify node G to red and modify
             * both node and node U to black, then set the node G as current node and
             * perform balanceAfterInsert operation on it.*/
            if (uNode != NULL && uNode->red)
            {
                gNode->red = true;
                uNode->red = false;
                current->parent->red = false;
                current = gNode;
                balanceAfterInsert(root, current);
            }
            else if (uNodeIsBlackOrNull(uNode))
            {
                /*
                * Case 4: If node P is red and node U is black, and the inserted node is the left
                * of node P, and the node P is the left of node G, then modify the node P to black,
                * and modify the node G to red. Finally, perform right rotate on node G and
                * balanceAfterInsert
                * on current node.
                */
                if (current == current->parent->left
                        && (gNode != NULL && current->parent ==
                            gNode->left))
                {
                    current->parent->red = false;
                    gNode->red = true;
                    rightRotate(&root, gNode);
                    balanceAfterInsert(root, current);
                }
                /*
                * Case 5: If node P is red and node U is black, and the inserted node is the right
                * of node P, and the node P is the right of node G, then modify the node P to
                * black, and modify the node G to red. Finally, perform left rotate on node G and
                * balanceAfterInsert on current node.
                */

                else if ( current == current->parent->right
                          && (gNode != NULL && current->parent ==
                              gNode->right))
                {
                    current->parent->red = false;
                    gNode->red = true;
                    leftRotate(&root, gNode);
                    balanceAfterInsert(root, current);
                }
                /*
                * Case 6: If node P is red and node U is black, and the inserted node is the right
                * of node P, and the node P is the left of node G, then let the node P be the new
                * current node. Finally, perform left rotate on the current node and
                * balanceAfterInsert on it
                */
                else if (current == current->parent->right
                         && (gNode != NULL && current->parent ==
                             gNode->left))
                {
                    RbNode* oldParent = current->parent;
                    leftRotate(&root, oldParent);
                    balanceAfterInsert(root, oldParent);
                }
                /*
                * Case 7:If node P is red and node U is black, and the inserted node is the left of
                * node P, and the node P is the right of node G, then let the node P be the new
                * current node. Finally perform right rotate on the current node and
                * balanceAfterInsert on it
                */
                else if (current == current->parent->left
                         && (gNode != NULL && current->parent ==
                             gNode->right))
                {
                    RbNode* oldParent = current->parent;
                    rightRotate(&root, oldParent);
                    balanceAfterInsert(root, oldParent);
                }

            }

        }

    }

}

static bool uNodeIsBlackOrNull(RbNode* uNode)
{
    return (uNode == NULL || !uNode->red);
}

static void leftRotate(RbNode** root, RbNode* pivot)
{
    RbNode* oldRight = pivot->right;
    RbNode* leftofOldRight = NULL;
    if (oldRight != NULL)
    {
        leftofOldRight = oldRight->left;
    }

    if (pivot->parent != NULL)
    {
        bool isLeft = (pivot->parent->left == pivot);
        if (isLeft)
        {
            pivot->parent->left = oldRight;
        }
        else
        {
            pivot->parent->right = oldRight;
        }

        if (oldRight != NULL)
        {
            oldRight->parent = pivot->parent;
        }
    }
    else
    {
        oldRight->parent = NULL;
        oldRight->red = false;
        *root = oldRight;
    }

    if (oldRight != NULL)
    {
        oldRight->left = pivot;
    }
    pivot->parent = oldRight;

    pivot->right = leftofOldRight;
    if (leftofOldRight != NULL)
    {
        leftofOldRight->parent = pivot;
    }
}

static void rightRotate(RbNode** root, RbNode* pivot)
{
    RbNode* oldLeft = pivot->left;
    RbNode* rightOfOldLeft = NULL;
    if (oldLeft != NULL)
    {
        rightOfOldLeft = oldLeft->right;
    }

    if (pivot->parent != NULL)
    {
        bool isLeft = (pivot->parent->left == pivot);
        if (isLeft)
        {
            pivot->parent->left = oldLeft;
        }
        else
        {
            pivot->parent->right = oldLeft;
        }
        if (oldLeft != NULL)
        {
            oldLeft->parent = pivot->parent;
        }
    }
    else
    {
        oldLeft->parent = NULL;
        oldLeft->red = false;
        *root = oldLeft;
    }

    if (oldLeft != NULL)
    {
        oldLeft->right = pivot;
    }
    pivot->parent = oldLeft;

    pivot->left = rightOfOldLeft;
    if (rightOfOldLeft != NULL)
    {
        rightOfOldLeft->parent = pivot;
    }
}
