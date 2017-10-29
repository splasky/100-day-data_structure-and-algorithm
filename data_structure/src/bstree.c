#include "../include/bstree.h"
#include "../include/dbg.h"
#include <stdio.h>
#include <stdlib.h>

static int int_compare(const void* restrict a, const void* restrict b)
{
    const int x = *((int*)(a));
    const int y = *((int*)b);
    return (x > y) - (y > x);
}

BSTree* New_BStree(BSTree_compare bstree_compare)
{
    BSTree* bstree = calloc(1, sizeof(BSTree));
    check_mem(bstree);

    bstree->count = 0;
    bstree->compare = bstree_compare == NULL ? int_compare : bstree_compare;
    bstree->root = NULL;
    return bstree;
error:
    log_err("create bstree failed");
    return NULL;
}

int BSTreeNode_destory(BSTreeNode* node)
{
    free(node);
    node = NULL;
    return 0;
}

BSTreeNode* BSTreeNode_create(BSTreeNode* parent, void* key, void* value)
{
    BSTreeNode* node = calloc(1, sizeof(BSTreeNode));
    check_mem(node);
    node->parent = parent;
    node->key = key;
    node->data = value;
    return node;
error:
    log_err("create bstree node error.");
    return NULL;
}

void BSTree_destory(BSTree* bstree)
{
    if (bstree) {
        BSTree_traverse(bstree, BSTreeNode_destory);
        free(bstree);
        bstree = NULL;
    }
}

static inline void BSTreeNode_set(BSTree* bstree, BSTreeNode* node, void* key, void* data)
{

    if (bstree->compare(node->key, key) <= 0) {
        /* insert left */
        if (node->left) {
            BSTreeNode_set(bstree, node->left, key, data);
        } else {
            node->left = BSTreeNode_create(node, key, data);
        }
    } else {
        if (node->right) {
            BSTreeNode_set(bstree, node->right, key, data);
        } else {
            node->right = BSTreeNode_create(node, key, data);
        }
    }
}

int BSTree_set(BSTree* bstree, void* key, void* data)
{

    if (!bstree->root) {
        BSTreeNode* node = BSTreeNode_create(NULL, key, data);
        check_mem(node);
        bstree->root = node;
        return 0;
    } else {
        BSTreeNode_set(bstree, bstree->root, key, data);
    }

error:
    log_err("set node failed");
    return -1;
}

static inline BSTreeNode* BSTreeNode_get_node(BSTree* bstree, BSTreeNode* node, void* key)
{
    int compare = bstree->compare(node->key, key);
    if (compare == 0) {
        return node;
    } else if (compare > 0 && node->right) {
        return BSTreeNode_get_node(bstree, node->right, key);
    } else if (compare < 0 && node->left) {
        return BSTreeNode_get_node(bstree, node->left, key);
    }
    return NULL;
}

void* BSTree_get(BSTree* bstree, void* key)
{
    if (!bstree->root)
        return NULL;

    BSTreeNode* node = BSTreeNode_get_node(bstree, bstree->root, key);
    return node == NULL ? NULL : node->data;
}

static inline int BSTreeNode_traverse(
    BSTreeNode* node, BSTree_traverse_cb bstree_traverse)
{
    int rc = 0;
    if (node->left) {
        rc = BSTreeNode_traverse(node->left, bstree_traverse);
        if (rc != 0)
            return rc;
    }
    if (node->right) {
        rc = BSTreeNode_traverse(node->right, bstree_traverse);
        if (rc != 0)
            return rc;
    }

    return bstree_traverse(node);
}

int BSTree_traverse(BSTree* bstree, BSTree_traverse_cb bstree_traverse)
{
    if (bstree->root) {
        return BSTreeNode_traverse(bstree->root, bstree_traverse);
    }

    return 0;
}
void* BSTree_delete(BSTree* bstree, void* key);
