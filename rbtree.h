#ifndef RBTREE_H_SGKRD6D2
#define RBTREE_H_SGKRD6D2

#include <stdbool.h>

typedef struct RbNode
{
    int id;
    void* data;
    bool red;
    struct RbNode* parent;
    struct RbNode* left;
    struct RbNode* right;
} RbNode;


RbNode* New_RbNode(int id, void* data);
void* RbNode_getData(RbNode*);
void RbNode_setData(RbNode* rb_node, void* data);
bool RbNode_isRed(RbNode*);
void RbNode_setRed(RbNode*, bool);
RbNode* RbNode_getParaent(RbNode*);
void RbNode_setParaent(RbNode* rb_node, RbNode* paraent);
RbNode* RbNode_getLeft(RbNode*);
void RbNode_setLeft(RbNode* rb_node, RbNode* left);
RbNode* RbNode_getRight(RbNode*);
void RbNode_setRight(RbNode* rb_node, RbNode* right);


#endif /* end of include guard: RBTREE_H_SGKRD6D2 */
