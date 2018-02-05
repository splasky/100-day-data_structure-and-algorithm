#include "../include/dbg.h"
#include "../include/list_algos.h"

static inline LinkedList* merge(
    LinkedList* left, LinkedList* right, LinkedList_Comparator cmp)
{
    LinkedList* result = New_LinkedList();
    void* val = NULL;

    while (LinkedList_count(left) > 0 || LinkedList_count(right) > 0) {
        if (LinkedList_count(left) > 0 && LinkedList_count(right) > 0) {
            if (cmp(LinkedList_first(left)->value, LinkedList_first(right)->value) <= 0) {
                val = LinkedList_shift(left);
            } else {
                val = LinkedList_shift(right);
            }
        } else if (LinkedList_count(left) > 0) {
            val = LinkedList_shift(left);
        } else if (LinkedList_count(right) > 0) {
            val = LinkedList_shift(right);
        }
        LinkedList_push(result, val);
    }

    return result;
}

LinkedList* LinkedList_merge_sort(LinkedList* list, LinkedList_Comparator comparator)
{
    if (LinkedList_count(list) <= 1) {
        return list;
    }

    int middle = LinkedList_count(list) / 2;
    LinkedList* a1 = New_LinkedList();
    LinkedList* a2 = New_LinkedList();

    LinkedList_Iterator iterator;
    LinkedList_InitializeIterator(&iterator, list);
    void* data = LinkedList_Iterate(&iterator);

    while (data != NULL) {
        if (middle > 0) {
            LinkedList_push(a1, data);
        } else {
            LinkedList_push(a2, data);
        }
        data = LinkedList_Iterate(&iterator);
        middle--;
    }

    LinkedList* soft_a1 = LinkedList_merge_sort(a1, comparator);
    LinkedList* soft_a2 = LinkedList_merge_sort(a2, comparator);

    if (soft_a1 != a1)
        LinkedList_destory(a1);

    if (soft_a2 != a2)
        LinkedList_destory(a2);

    LinkedList* out = merge(soft_a1, soft_a2, comparator);

    LinkedList_destory(soft_a1);
    LinkedList_destory(soft_a2);

    return out;
}
