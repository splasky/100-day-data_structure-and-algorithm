#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct ListNode ListNode;
typedef struct List LinkedList;

struct ListNode {
    void* data;
    ListNode* prev;
    ListNode* next;
};

struct List {
    int count;
    ListNode* tail;
    ListNode* head;
};

#define LinkedList_count(L) ((L)->count)
#define LinkedList_first(L) ((L)->head)
#define LinkedList_last(L) ((L)->tail)

LinkedList* New_LinkedList(void);
void LinkedList_destory(LinkedList* list);
void LinkedList_clear(LinkedList* list);
void LinkedList_clear_destory(LinkedList* list);
void LinkedList_push(LinkedList* list, void* data);
void* LinkedList_pop(LinkedList* list);
void* LinkedList_shift(LinkedList* list);
void* LinkedList_remove(LinkedList* list, ListNode* node);
void LinkedList_addFirst(LinkedList* list, void* value);

/*
 * L:list
 * S:first node
 * N:next or prev
 * C:current node
 */
#define LINKEDLIST_FOREACH(L, S, N, C)                                                   \
    ListNode* _node = NULL;                                                              \
    ListNode* C = NULL;                                                                  \
    for (C = _node = L->S; _node != NULL; C = _node = _node->N)
#endif /* ifndef LIST_H */
