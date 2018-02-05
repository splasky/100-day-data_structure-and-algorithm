#include "../include/btree.h"

static void BTree_erease_real(Btree* tree, Node* node, int key);
static void BTree_merge(Btree* tree, Node* node, int index);
static void BTree_split_child(Btree* self, Node* parent, int index, Node* child);
static void BTree_insert_non_full(Btree* self, Node* node, int key);

int Node_find_key(Node* self, int key)
{
    int idx = 0;
    while (idx < self->n && self->keys[idx] < key) {
        idx++;
    }
    return idx;
}

Node* New_Node(const int t, const bool leaf)
{
    Node* node = malloc(sizeof(Node));
    if (node == NULL) {
        goto error;
    }

    node->leaf = leaf;
    node->n = 0;
    /* TODO:not secure */
    node->keys = calloc(2 * t - 1, sizeof(int));
    node->children = calloc(2 * t, sizeof(Node));

    node->traverse = Node_traverse;
    node->search = Node_search;
    node->minial_degree = t;

    return node;
error:
    fprintf(stderr, "allocate new node failed\n");
    return NULL;
}

void Node_traverse(Node* self)
{
    int i = 0;
    for (; i < self->n; i++) {
        if (!self->leaf) {
            self->children[i]->traverse(self->children[i]);
        }
        printf("%d ", self->keys[i]);
    }
    printf("\n");

    /* print subtree rooted with last child */
    if (!self->leaf)
        self->children[i]->traverse(self->children[i]);
}

Node* Node_search(Node* self, int k)
{
    // Find the first key greater than or equal to k
    int i = 0;
    while (i < self->n && k > self->keys[i])
        i++;

    // If the found key is equal to k, return this node
    if (self->keys[i] == k)
        return self;

    // If key is not found here and this is a leaf node
    if (self->leaf)
        return NULL;

    // Go to the appropriate child
    return self->children[i]->search(self->children[i], k);
}

Btree* New_Btree(const int minial_degree)
{
    Btree* btree = calloc(1, sizeof(Btree));
    btree->root = NULL;
    btree->minial_degree = minial_degree;
    btree->insert = BTree_insert;
    btree->traverse = BTree_traverse;
    btree->search = BTree_search;

    return btree;
}

// TODO:not finish
void BTree_remove(Node* self)
{
    if (self->children[0]) {
        for (int i = 0; i < self->n; i++) {
            free(self->children[i]);
        }
    }
    free(self);
}

Node* BTree_search(const Btree* self, const int k)
{
    return (self->root == NULL) ? NULL : self->root->search(self->root, k);
}

void BTree_traverse(Btree* self)
{
    if (self->root != NULL)
        self->root->traverse(self->root);
}

/* split child node and create new parent when child's keys is full.
 * @param self Btree instance
 * @param parent New parents for child
 * @param index Move key into parent[index]
 * @param child The child to be splited
 */
void BTree_split_child(Btree* self, Node* parent, int index, Node* child)
{

    const int btree_degree = self->minial_degree;
    /* create new node for move last half child into tmp */
    Node* tmp = New_Node(btree_degree, child->leaf);
    /* setting numbers of child */
    tmp->n = btree_degree - 1;

    /* move last half keys and child into tmp */
    for (int j = 0; j < btree_degree - 1; j++) {
        tmp->keys[j] = child->keys[j + btree_degree];
    }
    if (!child->leaf) {
        for (int i = 0; i < btree_degree; ++i) {
            tmp->children[i] = child->children[i + btree_degree];
        }
    }

    /* reduce the number of keys in child */
    child->n = btree_degree - 1;

    /* move space for middle key */
    for (int i = parent->n; i >= index + 1; i--) {
        parent->children[i + 1] = parent->children[i];
    }

    /* Link a new child to parent node */
    parent->children[index + 1] = tmp;

    /* A key of child will move to parent.
     * Find location of new key and move all greater keys
     * one space ahead.
     */
    for (int i = parent->n - 1; i >= index; i--) {
        parent->keys[i + 1] = parent->keys[i];
    }
    /* copy the middle key of y to this node */
    parent->keys[index] = child->keys[btree_degree - 1];

    parent->n++;
}

void BTree_insert_non_full(Btree* self, Node* node, int key)
{
    /* index of right most elements */
    int i = node->n - 1;
    if (node->leaf) {
        /* find position to insert and move all greater keys to one place ahead */
        while (i >= 0 && node->keys[i] > key) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        /* don't insert duplicate key */
        if (node->keys[i] != key) {
            node->keys[i + 1] = key;
            node->n++;
        }

    } else {
        while (i >= 0 && node->keys[i] > key) {
            i--;
        }

        if (node->keys[i] != key) {
            /* if child is full ,split the child*/
            if (node->children[i + 1]->n == 2 * self->minial_degree - 1) {
                BTree_split_child(self, node, i + 1, node->children[i + 1]);
                if (node->keys[i + 1] < key) {
                    ++i;
                }
            }

            BTree_insert_non_full(self, node->children[i + 1], key);
        }
    }
}

void BTree_insert(Btree* self, int key)
{
    if (self->root != NULL) {
        /* if root if full */
        if (self->root->n == 2 * self->minial_degree - 1) {
            /* s is new parent */
            Node* s = New_Node(self->minial_degree, false);
            /* make old root as child of new root */
            s->children[0] = self->root;
            BTree_split_child(self, s, 0, self->root);

            int i = 0;
            if (s->keys[0] < key)
                i++;

            if (s->keys[0] != key)
                BTree_insert_non_full(self, s->children[i], key);

            self->root = s;
        } else {
            BTree_insert_non_full(self, self->root, key);
        }
    } else {
        /* create root */
        self->root = New_Node(self->minial_degree, true);
        self->root->keys[0] = key;
        self->root->n = 1;
    }
}
void BTree_erase(Btree* self, int key)
{
    if (self->root) {
        return;
    }

    BTree_erease_real(self, self->root, key);

    if (self->root->n == 0) {
        Node* tmp = self->root;

        if (self->root->leaf) {
            self->root = NULL;
        } else {
            self->root = self->root->children[0];
        }
        free(tmp);
    }
}

static void BTree_erease_real(Btree* self, Node* node, int key)
{
    /* index is key's position */
    int index = 0;
    index = node->find_key(node, key);

    if (index < node->n && node->keys[index] == key) {
        if (node->leaf) {
            for (int i = index + 1; i < node->n; ++i) {
                node->keys[i - 1] = node->keys[i];
            }
            node->n--;
        }
        /* if node is not leaf */
        else {
            /* if left child numbers of children more than minial_degree,
             * than use pred*/
            if (node->children[index]->n >= self->minial_degree) {
                Node* pred = node->children[index];
                while (!pred->leaf) {
                    pred = pred->children[pred->n - 1];
                }

                int precursor_key = pred->keys[pred->n - 1];
                node->keys[index] = precursor_key;

                BTree_erease_real(self, node->children[index], precursor_key);
            } else if (node->children[index + 1]->n >= self->minial_degree) {

                Node* succ = node->children[index + 1];
                while (!succ->leaf) {
                    succ = succ->children[0];
                }

                int succ_key = succ->keys[0];
                node->keys[index] = succ_key;

                BTree_erease_real(self, node->children[index + 1], succ_key);
            } else {
                BTree_merge(self, node, index);
                BTree_erease_real(self, node->children[index], key);
            }
        }

    } else {
        if (node->leaf) {
            printf("%d is not in tree!", key);
            return;
        }

        bool flag = (index == node->n) ? true : false;

        if (node->children[index]->n == self->minial_degree - 1) {
            /* if left child's keys more than minial_degree-1 */
            /* borrow from previous */
            if (index != 0 && node->children[index - 1]->n > self->minial_degree - 1) {
                Node* cur = node->children[index];
                Node* pre = node->children[index - 1];

                /* keys shift right */
                for (int j = cur->n - 1; j >= 0; j--)
                    cur->keys[j + 1] = cur->keys[j];

                if (!cur->leaf) {
                    /* children shift right */
                    for (int j = cur->n; j >= 0; j--)
                        cur->children[j + 1] = cur->children[j];

                    cur->children[0] = pre->children[pre->n];
                }

                /* move middle index to cur */
                cur->keys[0] = node->keys[index - 1];
                node->keys[index - 1] = pre->keys[pre->n - 1];
                cur->n++;
                pre->n--;
            }
            /* right child's keys more than minial_degree-1 */
            /* borrow from next */
            else if (index != node->n
                && node->children[index + 1]->n > self->minial_degree - 1) {
                Node* cur = node->children[index];
                Node* next = node->children[index + 1];

                for (int j = 1; j < next->n; j++)
                    next->keys[j - 1] = next->keys[j];

                if (!next->leaf) {
                    for (int j = 1; j < next->n; j++)
                        next->children[j - 1] = next->children[j];

                    cur->children[cur->n + 1] = next->children[0];
                }

                cur->keys[cur->n] = node->keys[index];
                node->keys[index] = next->keys[0];
                cur->n++;
                next->n--;
            }
            /* Merge children[index] with it siblings.
             * If children[index] is the last child, merge it with it previous siblings.
             * else merge with next siblings.
             */
            else {
                if (index != node->n)
                    BTree_merge(self, node, index);
                else
                    BTree_merge(self, node, index - 1);
            }
        }

        if (flag && index > node->n)
            BTree_erease_real(self, node->children[index - 1], key);
        else
            BTree_erease_real(self, node->children[index], key);
    }
}

static void BTree_merge(Btree* tree, Node* node, int index)
{
    const int t = tree->minial_degree;
    Node* curr = node->children[index];
    Node* next = node->children[index + 1];

    /* move index key into curr middle */
    curr->keys[t - 1] = node->keys[index];

    /* move next children and keys into curr */

    for (int i = 0; i < next->n; i++) {
        curr->keys[i + t] = next->keys[i];
    }

    if (!next->leaf) {
        for (int i = 0; i < next->n; i++) {
            curr->children[i + t] = next->children[i];
        }
    }

    /* shift left node keys and children */
    for (int i = index + 1; i < node->n; i++) {
        node->keys[i - 1] = node->keys[i];
    }

    for (int i = index + 2; i < node->n; i++) {
        node->children[i - 1] = node->children[i];
    }

    /* update curr and node key numbers */
    curr->n += next->n + 1;
    node->n--;
    free(next);
}
