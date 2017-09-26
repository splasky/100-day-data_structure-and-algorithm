#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct Node_ {
    void* data;
    struct _Node* prev;
    struct _Node* next;
} Node;

typedef struct List_ {
    int size;
    Node* tail;
    Node* head;
    void (*destory)(struct List_* list);
} List;

List* init_link_list(List* list);
void list_destory(List* list);
void list_remove(List* list, Node node);
void list_push(List* list, Node node);
void* list_pop(List* list);
void* list_peek(List* list);

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
