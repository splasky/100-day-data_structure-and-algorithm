#include "../include/list.h"
#include <stdlib.h>

LinkedList* New_LinkedList(void) { return calloc(1, sizeof(LinkedList)); }

void LinkedList_destory(LinkedList* list)
{
    LINKEDLIST_FOREACH(list, head, next, cur)
    {
        if (cur->prev) {
            free(cur->prev);
        }
    }
    free(LinkedList_last(list));
    free(list);
}

void LinkedList_clear(LinkedList* list)
{
    LINKEDLIST_FOREACH(list, head, next, cur) { free(cur->prev); }
}
void LinkedList_clear_destory(LinkedList* list)
{
    LinkedList_clear(list);
    LinkedList_destory(list);
}
void LinkedList_push(LinkedList* list, void* data)
{
    ListNode* node = calloc(1, sizeof(ListNode));
    node->data = data;

    if (LinkedList_last(list) == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        node->prev = list->tail;
        list->tail->next = node;
        list->tail = node;
    }
    list->count++;
}

void* LinkedList_pop(LinkedList* list)
{
    ListNode* last = LinkedList_last(list);
    return (last != NULL) ? LinkedList_remove(list, last) : NULL;
}

void* LinkedList_shift(LinkedList* list)
{
    return (list->head != NULL) ? LinkedList_remove(list, list->head) : NULL;
}

void LinkedList_addFirst(LinkedList* list, void* value)
{
    ListNode* node = calloc(1, sizeof(ListNode));
    node->data = value;

    if (LinkedList_first(list) == NULL) {
        list->head = node;
        list->tail = node;
    } else {
        node->next = LinkedList_first(list);
        LinkedList_first(list)->prev = node;
        list->head = node;
    }
    list->count++;
}

void* LinkedList_remove(LinkedList* list, ListNode* node)
{
    void* result = NULL;

    if (LinkedList_first(list) == node && LinkedList_last(list) == node) {
        list->head = NULL;
        list->tail = NULL;
    } else if (LinkedList_first(list) == NULL) {
        list->head = list->head->next;
        list->head->prev = NULL;
    } else if (LinkedList_last(list) == NULL) {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    } else {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    list->count--;
    result = node->data;
    free(node);

    return result;
}
