#include "../include/rbtree.h"
#include <stdlib.h>
#include "../include/dbg.h"

static void balanceAfterInsert(RbNode** root, RbNode* newNode);
static void leftRotate(RbNode** root, RbNode* pivot);
static void rightRotate(RbNode** root, RbNode* pivot);
static int colorOf(RbNode* p);
static RbNode* parentOf(RbNode* p);
static void setColor(RbNode* p, int color);
static RbNode* leftOf(RbNode* p);
static RbNode* rightOf(RbNode* p);
static void treePrintIndented(const RbNode* root, int depth, RbTree_print print);
static void fixDeleteRBTree(RbTree* rbtree, RbNode *node);

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
    RbNode_insert(&rbtree->root, id, data, rbtree->compare);
}

void RbNode_free(RbNode* node)
{
    /* You must check the node has no left child
     * and right child
     */
    if (!node)
    {
        return;
    }

    if (node->parent)
    {
        if (node->parent->left == node)
        {
            node->parent->left = NULL;
        }
        if (node->parent->right == node)
        {
            node->parent->right = NULL;
        }

    }

    free(node->data);
    free(node->id);
    free(node);
}

static void fixDeleteRBTree(RbTree* rbtree, RbNode *node)
{
    RbNode* root = rbtree->root;
    if (node == NULL)
        return;


    if (colorOf(node) == RED || colorOf(node->left) == RED || colorOf(node->right) == RED)
    {
        RbNode *child = node->left != NULL ? node->left : node->right;

        if (node == node->parent->left)
        {
            node->parent->left = child;
            if (child != NULL)
                child->parent = node->parent;
            setColor(child, BLACK);
            RbNode_free (node);
        }
        else
        {
            node->parent->right = child;
            if (child != NULL)
                child->parent = node->parent;
            setColor(child, BLACK);
            RbNode_free (node);
        }
    }
    else
    {
        RbNode *sibling = NULL;
        RbNode *parent = NULL;
        RbNode *ptr = node;
        setColor(ptr, DOUBLE_BLACK);
        while (ptr != root && colorOf(ptr) == DOUBLE_BLACK)
        {
            parent = parentOf(ptr);
            if (ptr == leftOf(parent))
            {
                sibling = rightOf(parent);
                if (colorOf(sibling) == RED)
                {
                    setColor(sibling, BLACK);
                    setColor(parent, RED);
                    leftRotate(&root, parent);
                }
                else
                {
                    if (colorOf(leftOf(sibling)) == BLACK &&
                            colorOf(rightOf(sibling)) == BLACK)
                    {
                        setColor(sibling, RED);
                        if (colorOf(parent) == RED)
                            setColor(parent, BLACK);
                        else
                            setColor(parent, DOUBLE_BLACK);
                        ptr = parent;
                    }
                    else
                    {
                        if (colorOf(rightOf(sibling)) == BLACK)
                        {
                            setColor(leftOf(sibling), BLACK);
                            setColor(sibling, RED);
                            rightRotate(&root, sibling);
                            sibling = rightOf(parent);
                        }
                        setColor(sibling, colorOf(parent));
                        setColor(parent, BLACK);
                        setColor(rightOf(sibling), BLACK);
                        leftRotate(&root, parent);
                        break;
                    }
                }
            }
            else
            {
                sibling = leftOf(parent);
                if (colorOf(sibling) == RED)
                {
                    setColor(sibling, BLACK);
                    setColor(parent, RED);
                    rightRotate(&root, parent);
                }
                else
                {
                    if (colorOf(leftOf(sibling)) == BLACK
                            && colorOf(rightOf(sibling)) == BLACK)
                    {
                        setColor(sibling, RED);
                        if (colorOf(parent) == RED)
                            setColor(parent, BLACK);
                        else
                            setColor(parent, DOUBLE_BLACK);
                        ptr = parent;
                    }
                    else
                    {
                        if (colorOf(leftOf(sibling)) == BLACK)
                        {
                            setColor(rightOf(sibling), BLACK);
                            setColor(sibling, RED);
                            leftRotate(&root, sibling);
                            sibling = leftOf(parent);
                        }
                        setColor(sibling, colorOf(parent));
                        setColor(parent, BLACK);
                        setColor(leftOf(sibling), BLACK);
                        rightRotate(&root, parent);
                        break;
                    }
                }
            }
        }
        RbNode_free (node);
        setColor(root, BLACK);
        rbtree->root = root;
    }
}

static RbNode* minValueNode(RbNode *node)
{

    RbNode *ptr = node;

    while (ptr->left != NULL)
        ptr = ptr->left;

    return ptr;
}

bool RbTree_delete(RbTree* rbtree, void* id)
{
    RbNode* node = RbTree_getNode(rbtree, id);
    /* if is root */
    if (!node)
        return false;

    if (node == rbtree->root)
    {
        if (leftOf(node))
        {
            node->right->parent = NULL;
            rbtree->root = node->left;
            setColor(rbtree->root, BLACK);
        }
        else if (rightOf(node))
        {
            node->right->parent = NULL;
            rbtree->root = node->right;
            setColor(rbtree->root, BLACK);
        }
        else
        {
            rbtree->root = NULL;
        }
        RbNode_free(node);
        return true;
    }

    fixDeleteRBTree(rbtree, node);
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
        root = rbtree->compare(root->id, id) > 0 ? root->left : root->right;
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
    node->color = RED;
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

void RbNode_insert(RbNode** root, void* id, void* data, RbTree_compare compare)
{
    RbNode* newNode = New_RbNode(id, data);

    if (*root == NULL)
    {
        *root = newNode;
        (*root)->color = BLACK;
        return;
    }
    else
    {
        RbNode* current = *root;
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

void RbTree_printTree(RbTree* rbtree, RbTree_print print)
{

    treePrintIndented(rbtree->root, 0, print);
}

#define INDENTATION_LEVEL (2)

static void treePrintIndented(const RbNode* root, int depth, RbTree_print print)
{
    if (root)
    {
        /* print right subtree */
        treePrintIndented(root->right, depth + 1, print);
        for (int i = 0; i < INDENTATION_LEVEL * depth; ++i)
        {
            printf(" ");
        }
        print(root);
        /* print left subtree */
        treePrintIndented(root->left, depth + 1, print);
    }
}


static void balanceAfterInsert(RbNode** root, RbNode* current)
{
    while (current->parent != NULL && current->parent->color == RED)
    {

        if (current->parent == current->parent->parent->left)
        {
            RbNode* uncle = current->parent->parent->right;
            // case 1:if uncle is red
            if (uncle != NULL && uncle->color == RED)
            {
                current->parent->color = BLACK;
                uncle->color = BLACK;
                current->parent->parent->color = RED;
                current = current->parent->parent;
            }
            /* case 2 & 3: uncle is black */
            else
            {
                if (current == current->parent->right)
                {
                    current = current->parent;
                    leftRotate(root, current);
                }
                /* case 3 */
                current->parent->color = BLACK;
                current->parent->parent->color = RED;
                rightRotate(root, current->parent->parent);
            }
        }
        /* mirror with previous */
        else
        {
            RbNode* uncle = current->parent->parent->left;
            // case 1:if uncle is red
            if (uncle != NULL && uncle->color == RED)
            {
                current->parent->color = BLACK;
                uncle->color = BLACK;
                current->parent->parent->color = RED;
                current = current->parent->parent;
            }
            /* case 2 & 3: uncle is black */
            else
            {
                if (current == current->parent->left)
                {
                    current = current->parent;
                    rightRotate(root, current);
                }
                /* case 3 */
                current->parent->color = BLACK;
                current->parent->parent->color = RED;
                leftRotate(root, current->parent->parent);
            }
        }
    }
    (*root)->color = BLACK;
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
        oldRight->color = BLACK;
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
        oldLeft->color = BLACK;
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

static int colorOf(RbNode* p)
{
    return (p == NULL ? BLACK : p->color);
}

static RbNode* parentOf(RbNode* p)
{
    return (p == NULL ? NULL : p->parent);
}

static void setColor(RbNode* p, int color)
{
    if (p != NULL)
    {
        p->color = color;
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
