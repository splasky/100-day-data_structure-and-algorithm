#ifndef AVLTREE_H_ZECVIFQP
#define AVLTREE_H_ZECVIFQP

#include <stdlib.h>

#define LEFT (0)
#define RIGHT (1)
#define TREE_NUM_CHILDREN (2)

struct avltree {
    struct avltree* child[TREE_NUM_CHILDREN];
    int key;
    int height;
    size_t size;
};

#define TREE_EMPTY (0)
#define TREE_EMPTY_HEIGHT (-1)

void avltreeDestroy(struct avltree** root);
void avltreeInsert(struct avltree** root, int new_element);
int avltreeContains(const struct avltree* root, int target);
int avltreeDeleteMin(struct avltree** root);
void avltreeDelete(struct avltree** root, int target);
int avltreeHeight(const struct avltree* root);
size_t avltreeSize(const struct avltree* root);
void avltreePrint(const struct avltree* root);
size_t avltreeRank(const struct avltree* root, int target);
int avltreeUnrank(const struct avltree* root, size_t rank);
void avltreeSanityCheck(const struct avltree* root);

#endif /* end of include guard: AVLTREE_H_ZECVIFQP */
