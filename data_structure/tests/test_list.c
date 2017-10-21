#include "../include/list.h"
#include "unit.h"

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

TEST(test_clear_destory)
{
    LinkedList* llist = New_LinkedList();
    int a[] = { 1, 2, 3, 4 };
    int* p = a;
    LinkedList_push(llist, (void*)p);
    LinkedList_push(llist, (void*)(p + 1));
    LinkedList_push(llist, (void*)(p + 2));
    LinkedList_push(llist, (void*)(p + 3));
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
    int arr[] = { 1, 2, 3, 4, 5 };
    int* p = arr;

    LinkedList_push(list, (void*)(p));
    LinkedList_push(list, (void*)(p + 1));
    LinkedList_push(list, (void*)(p + 2));
    LinkedList_push(list, (void*)(p + 3));
    LinkedList_push(list, (void*)(p + 4));

    int num = 3;
    int* ptr = &num;

    LinkedList_addWithIndex(list, 3, ptr);

    int* curr_ptr = LinkedList_remove_index(list, 3);
    unit_assert((*curr_ptr) == 4, "Wrong on index");

    return NULL;
}

TEST(test_add_all)
{
    LinkedList* list1 = New_LinkedList();
    LinkedList* list2 = New_LinkedList();

    int arr[] = { 1, 2, 3, 4, 5 };
    int* p = arr;

    LinkedList_push(list1, (void*)(p));
    LinkedList_push(list1, (void*)(p + 1));
    LinkedList_push(list1, (void*)(p + 2));
    LinkedList_push(list1, (void*)(p + 3));
    LinkedList_push(list1, (void*)(p + 4));

    LinkedList_push(list2, (void*)(p));
    LinkedList_push(list2, (void*)(p + 1));
    LinkedList_push(list2, (void*)(p + 2));
    LinkedList_push(list2, (void*)(p + 3));
    LinkedList_push(list2, (void*)(p + 4));

    LinkedList_addALLWithIndex(list1, 3, list2);

    int* curr_value = LinkedList_remove_index(list1, 3);
    unit_assert((*curr_value) == 3, "Wrong on addALLWithIndex");
    LinkedList_clear_destory(list1);

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
