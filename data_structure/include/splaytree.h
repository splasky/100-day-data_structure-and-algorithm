#ifndef SPLAYTREE_H_JE1QX2ZS
#define SPLAYTREE_H_JE1QX2ZS

#define LEFT (0)
#define RIGHT (1)
#define TREE_NUM_CHILDREN (2)

struct splaytree {
    struct splaytree* child[TREE_NUM_CHILDREN];
    int key;
};

#define TREE_EMPTY (0)

void splaytreeDestroy(struct splaytree** root);
void splaytreeDelete(struct splaytree** root, int key);
void splaytreeInsert(struct splaytree** root, int new_element);
int splaytreeContains(struct splaytree** root, int key);
void splaytreePrint(const struct splaytree* root);

#endif /* end of include guard: SPLAYTREE_H_JE1QX2ZS */
