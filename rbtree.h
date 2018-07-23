#ifndef RBTREE_H_SGKRD6D2
#define RBTREE_H_SGKRD6D2

#include <stdbool.h>

typedef struct RbNode
{
    void* id;
    void* data;
    bool red;
    struct RbNode* parent;
    struct RbNode* left;
    struct RbNode* right;
} RbNode;

typedef int (*RbTree_compare)(void* a, void* b);
typedef struct RbTree
{
    RbNode* root;
    RbNode* height;
    RbTree_compare compare;
} RbTree;

RbNode* New_RbNode(void* id, void* data);
/* rbtree insert */
void RbNode_insert(RbNode* root, void* id, void* data, RbTree_compare);

RbTree* New_RbTree(RbTree_compare compare);
void RbTree_insert(RbTree* rbtree, void* id, void* data);
void RbTree_delete(RbTree* rbtree, void* id);
void RbTree_update(RbTree* rbtree, void* id, void* data);
void* RbTree_get(RbTree* rbtree, void* id);
void RbTree_destroy(RbTree* rbtree);
void* RbTree_getNode(RbTree* rbtree, void* id);


#endif /* end of include guard: RBTREE_H_SGKRD6D2 */
