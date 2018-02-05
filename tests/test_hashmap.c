#include "../include/hashmap.h"
#include "unit.h"

static int traverse_print(Hashmap_node* node)
{
    debug("Key %s", *(char**)node->data);
    return 1;
}

TEST(test_hashmap)
{

    Hashmap* map = Hashmap_create(NULL, NULL);
    unit_assert(map != NULL, "Failed to create map.");

    static const char* s1 = "Hello world";
    static const char* s2 = "Ya Ya Ya";
    static const char* s3 = "Have a nice day";

    static const char* key1 = "s1";
    static const char* key2 = "s2";
    static const char* key3 = "s3";

    int rc = 0;
    void* result;
    rc = Hashmap_set(map, &key1, &s1);
    unit_assert(rc == 0, "Failed to set s1");
    result = Hashmap_get(map, &key1);
    unit_assert(strcmp(*(char**)result, s1) == 0, "Wrong value for s1.");

    rc = Hashmap_set(map, &key2, &s2);
    unit_assert(rc == 0, "Failed to set s2");
    result = Hashmap_get(map, &key2);
    unit_assert(strcmp(*(char**)result, s2) == 0, "Wrong value for s2.");

    rc = Hashmap_set(map, &key3, &s3);
    unit_assert(rc == 0, "Failed to set s3");
    result = Hashmap_get(map, &key3);
    unit_assert(strcmp(*(char**)result, s3) == 0, "Wrong value for s3.");

    rc = Hashmap_traverse(map, traverse_print);
    unit_assert(rc == 0, "Failed to traverse.");

    result = Hashmap_remove(map, &key1);
    unit_assert(*(char**)result != NULL, "Got NULL on delete");
    unit_assert(strcmp(*(char**)result, s1) == 0, "Should get s1");
    result = Hashmap_get(map, &key1);
    unit_assert(result == NULL, "Should deleted");

    result = Hashmap_remove(map, &key2);
    unit_assert(*(char**)result != NULL, "Got NULL on delete");
    unit_assert(strcmp(*(char**)result, s2) == 0, "Should get s2");
    result = Hashmap_get(map, &key2);
    unit_assert(result == NULL, "Should deleted");

    result = Hashmap_remove(map, &key3);
    unit_assert(*(char**)result != NULL, "Got NULL on delete");
    unit_assert(strcmp(*(char**)result, s3) == 0, "Should get s3");
    result = Hashmap_get(map, &key3);
    unit_assert(result == NULL, "Should deleted");

    Hashmap_destroy(map);
    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(test_hashmap);
    return NULL;
}

RUN_TESTS(all_tests);
