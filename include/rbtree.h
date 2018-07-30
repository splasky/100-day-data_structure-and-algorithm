#ifndef RBTREE_H_SGKRD6D2
#define RBTREE_H_SGKRD6D2

#include <stdbool.h>

enum Color {RED, BLACK, DOUBLE_BLACK};

typedef struct RbNode
{
    void* id;
    void* data;
    int color;
    struct RbNode* parent;
    struct RbNode* left;
    struct RbNode* right;
} RbNode;

typedef int (*RbTree_compare)(const void* a, const void* b);
typedef void (*RbTree_print)(const RbNode* node);
typedef struct RbTree
{
    RbNode* root;
    int height;
    RbTree_compare compare;
} RbTree;

RbNode* New_RbNode(void* id, void* data);
/* rbtree insert */
void RbNode_insert(RbNode** root, void* id, void* data, RbTree_compare);
void RbNode_free(RbNode* node);

RbTree* New_RbTree(RbTree_compare compare);
void RbTree_insert(RbTree* rbtree, void* id, void* data);
bool RbTree_delete(RbTree* rbtree, void* id);
void RbTree_update(RbTree* rbtree, void* id, void* data);
void* RbTree_get(RbTree* rbtree, void* id);
void RbTree_destroy(RbTree* rbtree);
void* RbTree_getNode(RbTree* rbtree, void* id);
void RbTree_printTree(RbTree* rbtree, RbTree_print);

#endif /* end of include guard: RBTREE_H_SGKRD6D2 */
