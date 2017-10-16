#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct Node Node;
typedef struct List LinkedList;
struct Node {
    void* data;
    struct Node* prev;
    struct Node* next;
};

struct List {
    int size;
    Node* tail;
    Node* head;
    void (*destory)(LinkedList* list);
    int (*remove)(LinkedList* list, Node node);
    int (*push)(LinkedList* list, void* data);
    void* (*pop)(LinkedList* list);
    void* (*peek)(LinkedList* list);
    Node* (*search)(LinkedList* list, void* data);
};

LinkedList* init_link_list(LinkedList* list);
void list_destory(LinkedList* list);
void list_clear(LinkedList* list);
int list_remove(LinkedList* list, Node node);
int list_push(LinkedList* list, void* data);
void* list_pop(LinkedList* list);
void* list_peek(LinkedList* list);
Node* search(LinkedList* list, void* data);

/*
 * L:list
 * S:first node
 * M:next node
 * V:current node
 */
#define foreach(L, S, M, V)                                                              \
    Node* _node = NULL;                                                                  \
    Node* V = NULL;                                                                      \
    for (V = _node = L->S; _node != NULL; V = _node = _node->M)
#endif /* ifndef LIST_H */
