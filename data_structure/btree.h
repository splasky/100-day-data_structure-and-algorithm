#ifndef BTREE_H
#define BTREE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* #define Minimum_Degree 3; */

typedef struct Node Node;
struct Node {
    _Bool leaf; /* Is true when node is leaf */
    int n;      /* number of keys */
    int* keys;  /* keys */
    int minial_degree;
    Node** children;
    void (*traverse)(Node* self);
    Node* (*search)(Node* self, int k);
    int (*find_key)(Node* self, int k);
};

Node* New_Node(const int t, const _Bool leaf);
void Node_traverse(Node* self);
Node* Node_search(Node* self, int k);
int Node_find_key(Node* self, int key);

typedef struct Btree Btree;
static void BTree_split_child(Btree* self, Node* parent, int index, Node* child);
static void BTree_insert_non_full(Btree* self, Node* node, int key);
void BTree_insert(Btree* self, int key);
void BTree_traverse(Btree* self);
Node* BTree_search(const Btree* self, const int k);
void BTree_remove(Node* self);
void BTree_delete(Btree* self);
void BTree_erase(Btree* self, int key);
static void BTree_erease_real(Btree* tree, Node* node, int key);
static void BTree_merge(Btree* tree, Node* node, int index);

struct Btree {
    /* private */
    int minial_degree; /* minimal degree */
    Node* root;

    void (*insert)(Btree* self, int key);
    void (*traverse)(Btree* self);
    Node* (*search)(const Btree* self, const int key);
    void (*remove)(Btree* self, int k);
};

#endif /* ifndef BTREE_H */
