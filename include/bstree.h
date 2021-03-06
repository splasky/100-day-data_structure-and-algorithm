#ifndef BSTREE_H_LECXYVZA
#define BSTREE_H_LECXYVZA
typedef struct _BSTreeNode_t BSTreeNode;
struct _BSTreeNode_t {
    void* key;
    void* data;

    BSTreeNode* parent;
    BSTreeNode* left;
    BSTreeNode* right;
};

typedef struct BSTree_t BSTree;
typedef int (*BSTree_compare)(const void* a, const void* b);
struct BSTree_t {
    int count;
    BSTreeNode* root;
    BSTree_compare compare;
};

BSTree* New_BSTree(BSTree_compare bstree_compare);
void BSTree_destory(BSTree* bstree);

int BSTree_set(BSTree* bstree, void* restrict key, void* restrict data);
void* BSTree_get(BSTree* bstree, void* key);

typedef int (*BSTree_traverse_cb)(BSTreeNode* node);
typedef int (*BSTreeNode_traverse)(BSTreeNode* node, BSTree_traverse_cb bstree_traverse);
int BSTree_traverse(BSTree* bstree, BSTreeNode_traverse node_traverse,
    BSTree_traverse_cb bstree_traverse);
void* BSTree_delete(BSTree* bstree, void* key);

int BSTreeNode_traverse_preOrder(BSTreeNode* node, BSTree_traverse_cb bstree_traverse);
int BSTreeNode_traverse_postOrder(BSTreeNode* node, BSTree_traverse_cb bstree_traverse);
int BSTreeNode_traverse_inOrder(BSTreeNode* node, BSTree_traverse_cb bstree_traverse);
#endif /* end of include guard: BSTREE_H_LECXYVZA */
