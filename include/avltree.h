#ifndef AVLTREE_H_ZECVIFQP
#define AVLTREE_H_ZECVIFQP

#include <stdbool.h>
#include <stdlib.h>

#define LEFT (0)
#define RIGHT (1)
#define TREE_NUM_CHILDREN (2)

typedef int (*AVLTree_compare)(const void* a, const void* b);

typedef struct _AVLTREENODE {
    struct _AVLTREENODE* child[TREE_NUM_CHILDREN];
    void* key;
    void* data;
    int height;
    size_t size;
    AVLTree_compare compare;
} AVLTreeNode;

typedef void (*AVLTree_print)(const AVLTreeNode* root);

#define TREE_EMPTY (0)
#define TREE_EMPTY_HEIGHT (-1)

void avltreeDestroy(AVLTreeNode** root);
void avltreeInsert(AVLTreeNode** root, void* key, void* data, AVLTree_compare compare);
bool avltreeContains(const AVLTreeNode* root, const void* key);
void* avltreeFind(const AVLTreeNode* root, const void* key);
void* avltreeDeleteMin(AVLTreeNode** root);
void avltreeDelete(AVLTreeNode** root, void* target);
int avltreeHeight(const AVLTreeNode* root);
size_t avltreeSize(const AVLTreeNode* root);
void avltreePrint(const AVLTreeNode* root, AVLTree_print print);
void avltreeSanityCheck(const AVLTreeNode* root);

#endif /* end of include guard: AVLTREE_H_ZECVIFQP */
