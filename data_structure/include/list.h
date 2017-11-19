#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdlib.h>

typedef struct ListNode ListNode;
typedef struct List LinkedList;

struct ListNode {
    void* value;
    ListNode* prev;
    ListNode* next;
};

struct List {
    int count;
    ListNode* tail;
    ListNode* head;
};

#define LinkedList_count(L) ((L)->count)
#define LinkedList_first(L) (((L)->head != NULL) ? (L)->head : NULL)
#define LinkedList_last(L) (((L)->tail) != NULL ? (L)->tail : NULL)

LinkedList* New_LinkedList(void);
void LinkedList_destory(LinkedList* list);
void LinkedList_clear(LinkedList* list);
void LinkedList_clear_destory(LinkedList* list);
void LinkedList_push(LinkedList* list, void* value);
void* LinkedList_pop(LinkedList* list);
void* LinkedList_shift(LinkedList* list);
void* LinkedList_remove(LinkedList* list, ListNode* node);
void* LinkedList_remove_index(LinkedList* list, const int index);
void LinkedList_addFirst(LinkedList* list, void* value);
void LinkedList_addWithIndex(LinkedList* list, const int index, void* value);
void LinkedList_addALL(LinkedList* list, LinkedList* added);
void LinkedList_addALLWithIndex(LinkedList* list, const int index, LinkedList* listadd);
void* LinkedList_get(LinkedList* list, const int index);
/* TODO:need test */
typedef int (*LinkedList_Comparator)(const void* a, const void* b);
int LinkedList_item_exists(
    LinkedList* list, void* value, LinkedList_Comparator comparator);
int LinkedList_sorted(LinkedList* list, LinkedList_Comparator comparator);
LinkedList* LinkedList_deep_copy(LinkedList* list);
/* TODO:need implement and test*/
void* LinkedList_find_value(LinkedList* list, void* data);
ListNode* LinkedList_find_node(LinkedList* list, void* toFind);
void LinkedList_remove_data(LinkedList* list, void* data);

/*
 * L:list
 * S:first node
 * N:next or prev
 * C:current node
 */
/* TODO:_node change to hash */
#define LINKEDLIST_FOREACH(L, S, N, C)                                                   \
    ListNode* _node = NULL;                                                              \
    ListNode* C = NULL;                                                                  \
    for (C = _node = L->S; _node != NULL; C = _node = _node->N)
#endif /* ifndef LIST_H */
