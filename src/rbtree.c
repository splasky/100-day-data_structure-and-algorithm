#include "../include/rbtree.h"
#include <stdlib.h>
#include "../include/dbg.h"

static void balanceAfterInsert(RbNode* root, RbNode* newNode);
static bool uNodeIsBlackOrNull(RbNode* uNode);
static void leftRotate(RbNode** root, RbNode* pivot);
static void rightRotate(RbNode** root, RbNode* pivot);
static RbNode* successor(RbNode* node);
static void fixAfterDelection(RbTree* rbtree, RbNode* node);
static bool colorOf(RbNode* p);
static RbNode* parentOf(RbNode* p);
static void setColor(RbNode* p, bool color);
static RbNode* leftOf(RbNode* p);
static RbNode* rightOf(RbNode* p);

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

void RbNode_free(RbNode* node)
{
    /* You must check the node has no left child
     * and right child
     */

    if (node->parent->left == node)
    {
        node->parent->left = NULL;
    }
    else
    {
        node->parent->right = NULL;
    }
    free(node->data);
    free(node->id);
    free(node);
}

bool RbTree_delete(RbTree* rbtree, void* id)
{
    RbNode* node = RbTree_getNode(rbtree, id);
    if (node == NULL)
    {
        return false;
    }

    /* If node have two child, find the succesor */
    if (node->left != NULL && node->right != NULL)
    {
        RbNode* s = successor(node);
        node->data = s->data;
        node->id = s->id;
        node = s;
    }

    RbNode* replacement = (node->left != NULL ? node->left : node->right);
    if (replacement != NULL)
    {
        replacement->parent = node->parent;
        if (node->parent == NULL)
        {
            rbtree->root = replacement;
        }
        else if (node == node->parent->left)
        {
            node->parent->left = replacement;
        }
        else
        {
            node->parent->right = replacement;
        }
        node->left = node->right = node->parent = NULL;
        if (node->red == BLACK)
        {
            fixAfterDelection(rbtree, replacement);
        }
    }
    else if (node->parent == NULL)
    {
        rbtree->root = NULL;
    }
    else
    {
        /* no child */
        if (node->red == BLACK)
        {
            fixAfterDelection(rbtree, node);
        }
        if (node->parent != NULL)
        {
            if (node == node->parent->left)
            {
                node->parent->left = NULL;
            }
            else
            {
                node->parent->right = NULL;
            }
            node->parent = NULL;
        }
    }
    return true;
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
static RbNode* successor(RbNode* node)
{
    if (node == NULL)
    {
        return NULL;
    }

    if (node->right != NULL)
    {
        RbNode* p = node->right;
        while (p->left != NULL)
        {
            p = p->left;
        }
        return p;
    }
    else
    {
        RbNode* p = node->parent;
        RbNode* ch = node;
        while (p != NULL && ch == p->right)
        {
            ch = p;
            p = p->parent;
        }
        return p;
    }
}

static void fixAfterDelection(RbTree* rbtree, RbNode* node)
{
    RbNode* root = rbtree->root;
    while (node != root && colorOf(node) == BLACK)
    {
        if (node == leftOf(parentOf(node)))
        {

            RbNode* sib = rightOf(parentOf(node));

            if (colorOf(sib) == RED)
            {
                setColor(sib, BLACK);
                setColor(parentOf(node), RED);
                leftRotate(&root, parentOf(node));
                sib = rightOf(parentOf(node));
            }

            if (colorOf(leftOf(sib)) == BLACK &&
                    colorOf(rightOf(sib)) == BLACK)
            {
                setColor(sib, RED);
                node = parentOf(node);
            }
            else
            {
                if (colorOf(rightOf(sib)) == BLACK)
                {
                    setColor(leftOf(sib), BLACK);
                    setColor(sib, RED);
                    rightRotate(&root, sib);
                    sib = rightOf(parentOf(node));
                }
                setColor(sib, colorOf(parentOf(node)));
                setColor(parentOf(node), BLACK);
                setColor(rightOf(sib), BLACK);
                leftRotate(&root, parentOf(node));
                node = root;
            }// symmetric
        }
        else
        {
            RbNode* sib = leftOf(parentOf(node));
            if (colorOf(sib) == RED)
            {
                setColor(sib, BLACK);
                setColor(parentOf(node), RED);
                rightRotate(&root, parentOf(node));
                sib = leftOf(parentOf(node));
            }
            if (colorOf(rightOf(sib)) == BLACK &&
                    colorOf(leftOf(sib)) == BLACK)
            {
                setColor(sib, RED);
                node = parentOf(node);
            }
            else
            {
                if (colorOf(leftOf(sib)) == BLACK)
                {
                    setColor(rightOf(sib), BLACK);
                    setColor(sib, RED);
                    leftRotate(&root, sib);
                    sib = leftOf(parentOf(node));
                }
                setColor(sib, colorOf(parentOf(node)));
                setColor(parentOf(node), BLACK);
                setColor(leftOf(node), BLACK);
                rightRotate(&root, parentOf(node));
                node = root;

            }
        }
    }

    setColor(node, BLACK);
}

static bool colorOf(RbNode* p)
{
    return (p == NULL ? BLACK : p->red);
}

static RbNode* parentOf(RbNode* p)
{
    return (p == NULL ? NULL : p->parent);
}

static void setColor(RbNode* p, bool color)
{
    if (p != NULL)
    {
        p->red = color;
    }
}

static RbNode* leftOf(RbNode* p)
{
    return (p == NULL ? NULL : p->left);
}

static RbNode* rightOf(RbNode* p)
{
    return (p == NULL ? NULL : p->right);
}
