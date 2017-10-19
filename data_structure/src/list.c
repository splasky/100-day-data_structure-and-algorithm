#include "../include/list.h"
#include <stdio.h>
#include <stdlib.h>

static _Bool LinkedList_is_Over_Bound(LinkedList* list, int index);
static ListNode* LinkedList_getNodeFromIndex(LinkedList* list, int index);

static ListNode* LinkedList_getNodeFromIndex(LinkedList* list, int index)
{
    if (LinkedList_is_Over_Bound(list, index)) {
        goto error;
    }

    int i = 0;
    LINKEDLIST_FOREACH(list, head, next, cur)
    {
        if (i == index)
            break;
        ++i;
    }

    return cur;
error:
    return NULL;
}
static _Bool LinkedList_is_Over_Bound(LinkedList* list, int index)
{
    if (index > LinkedList_count(list)) {
        fprintf(stderr,
            "Insert into linked list out of bound(linked list length:%d index : %d)\n ",
            LinkedList_count(list), index);
        return true;
    }
    return false;
}

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

void LinkedList_push(LinkedList* list, void* value)
{
    ListNode* node = calloc(1, sizeof(ListNode));
    node->value = value;

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
    node->value = value;

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
    } else if (LinkedList_first(list) == node) {
        list->head = list->head->next;
        list->head->prev = NULL;
    } else if (LinkedList_last(list) == node) {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    } else {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }

    list->count--;
    result = node->value;
    free(node);

    return result;
}

void LinkedList_addWithIndex(LinkedList* list, const int index, void* value)
{

    ListNode* index_node = LinkedList_getNodeFromIndex(list, index);
    if (!index_node) {
        goto error;
    }

    if (LinkedList_first(list) == NULL) {
        LinkedList_push(list, value);
        return;
    }

    ListNode* node = calloc(1, sizeof(ListNode));
    node->value = value;

    node->prev = index_node->prev;
    node->next = index_node;
    index_node->prev = node;
    index_node->prev->next = node;

    list->count++;
error:
    return;
}

void LinkedList_addALL(LinkedList* list, LinkedList* added)
{
    list->tail->next = added->head;
    added->head->prev = list->tail;
    list->count += added->count;
    free(added);
}

void LinkedList_addALLWithIndex(LinkedList* list, const int index, LinkedList* added)
{
    ListNode* index_node = LinkedList_getNodeFromIndex(list, index);
    if (!index_node) {
        goto error;
    }

    index_node->next->prev = added->tail;
    added->tail->next = index_node->next;
    index_node->next = added->head;
    added->head->prev = index_node;

    list->count += added->count;
    free(added);

error:
    return;
}

void* LinkedList_remove_index(LinkedList* list, const int index)
{
    ListNode* index_node = LinkedList_getNodeFromIndex(list, index);
    if (!index_node) {
        goto error;
    }

    return LinkedList_remove(list, index_node);
error:
    return NULL;
}
