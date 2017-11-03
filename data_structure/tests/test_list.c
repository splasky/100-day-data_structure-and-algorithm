#include "../include/list.h"
#include "unit.h"
#include <stdint.h>

static LinkedList* list = NULL;
char* test1 = "test1";
char* test2 = "test2";
char* test3 = "test3";

TEST(test_create)
{
    list = New_LinkedList();
    unit_assert(list != NULL, "Create linked list error");
    return NULL;
}

TEST(test_destory)
{
    LinkedList_destory(list);
    return NULL;
}

LinkedList* SetTestingList(int size)
{
    LinkedList* llist = New_LinkedList();

    for (int i = 0; i < size; i++) {
        int* p = calloc(1, sizeof(int));
        *p = i;
        LinkedList_push(llist, p);
    }

    return llist;
}

TEST(test_clear_destory)
{
    LinkedList* llist = SetTestingList(10);
    LinkedList_clear_destory(llist);
    return NULL;
}

TEST(test_push_pop)
{
    LinkedList_push(list, test1);
    unit_assert(LinkedList_first(list)->value == test1, "Wrong first value");

    LinkedList_push(list, test2);
    unit_assert(LinkedList_count(list) == 2, "Wrong on count");

    LinkedList_push(list, test3);
    unit_assert(LinkedList_last(list)->value == test3, "Wrong last value");

    char* val = LinkedList_pop(list);
    unit_assert(val == test3, "Wrong on pop");

    val = LinkedList_pop(list);
    unit_assert(val == test2, "Wrong on pop");

    val = LinkedList_pop(list);
    unit_assert(val == test1, "Wrong on pop");
    unit_assert(LinkedList_count(list) == 0, "Wrong on count");

    return NULL;
}

TEST(test_shift)
{
    LinkedList_addFirst(list, test1);
    unit_assert(LinkedList_first(list)->value == test1, "Wrong on addFirst");

    LinkedList_addFirst(list, test2);
    unit_assert(LinkedList_first(list)->value == test2, "Wrong on addFirst");

    LinkedList_addFirst(list, test3);
    unit_assert(LinkedList_first(list)->value == test3, "Wrong on addFirst");

    char* val = LinkedList_shift(list);
    unit_assert(val == test3, "Wrong on addFirst");

    val = LinkedList_shift(list);
    unit_assert(val == test2, "Wrong on shift");

    val = LinkedList_shift(list);
    unit_assert(val == test1, "Wrong on shift");

    return NULL;
}

TEST(test_index)
{
    LinkedList* llist = SetTestingList(10);
    int num = 3;

    LinkedList_addWithIndex(llist, 3, (&num));

    int* curr_ptr = LinkedList_remove_index(llist, 3);
    unit_assert(*(curr_ptr) == 3, "Wrong on index");

    LinkedList_clear_destory(llist);
    return NULL;
}

TEST(test_add_all)
{
    LinkedList* list1 = SetTestingList(20);
    LinkedList* list2 = SetTestingList(30);

    int* val = LinkedList_get(list1, 3);
    unit_assert(*val == 3, "Wrong on push into list1");

    val = LinkedList_get(list2, 2);
    unit_assert(*val == 2, "Wrong on push into list2");

    LinkedList_addALLWithIndex(list1, 3, list2);

    val = LinkedList_remove_index(list1, 4);
    unit_assert(*(val) == 0, "Wrong on addALLWithIndex");
    free(val);

    LinkedList_clear_destory(list1);
    free(list2);
    list2 = NULL;

    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(test_create);
    unit_run_test(test_push_pop);
    unit_run_test(test_shift);
    unit_run_test(test_index);
    unit_run_test(test_clear_destory);
    unit_run_test(test_add_all);
    unit_run_test(test_destory);

    return NULL;
}

RUN_TESTS(all_tests);
