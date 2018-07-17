#include "rbtree.h"
#include <stdlib.h>

RbNode* New_RbNode(int id, void* data)
{
    RbNode* node = malloc(sizeof(RbNode));
    node->id = id;
    node->data = data;
    node->red = false;
    return node;
}

void* RbNode_getData(RbNode* rb_node)
{
    return rb_node->data;
}

void RbNode_setData(RbNode* rb_node, void* data)
{
    rb_node->data = data;
}

bool RbNode_isRed(RbNode* rb_node)
{
    return rb_node->red;
}

void RbNode_setRed(RbNode* rb_node, bool red)
{
    rb_node->red = red;
}

RbNode* RbNode_getParaent(RbNode* rb_node)
{
    return rb_node->parent;
}

void RbNode_setParaent(RbNode* rb_node, RbNode* parent)
{
    rb_node->parent = parent;
}

RbNode* RbNode_getLeft(RbNode* rb_node)
{
    return rb_node->left;
}

void RbNode_setLeft(RbNode* rb_node, RbNode* left)
{
    rb_node->left = left;
}

RbNode* RbNode_getRight(RbNode* rb_node)
{
    return rb_node->right;
}

void RbNode_setRight(RbNode* rb_node, RbNode* right)
{
    rb_node->right = right;
}
