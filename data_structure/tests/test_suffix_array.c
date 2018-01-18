#include "../include/dbg.h"
#include "../include/suffix_array.h"
#include "unit.h"

TEST(test_suffix_array)
{
    static const char* string = "Hello cheaee!";

    SuffixArray* suffix_array = suffixArrayCreate(string);
    size_t first, count;
    count = suffixArraySearch(suffix_array, "chea", &first);
    printf("Count: %zu\n", count);
    printf("First: %zu\n", first);
    unit_assert(count == 1, "Test search failed");
    unit_assert(first == 5, "Test search failed");

    char* bwt = suffixArrayBWT(suffix_array);
    printf("BWT: %s\n", bwt);
    unit_assert(!strcmp(bwt, "!oe"), "Test bwt failed");

    char* iwt = inverseBWT(suffix_array->n, bwt);
    printf("IWT: %s\n", iwt);
    unit_assert(!strcmp(iwt, "Hello cheaee!"), "Test iwt failed");

    free(bwt);
    free(iwt);
    suffixArrayDestroy(suffix_array);

    return NULL;
}

TEST(all_tests)
{
    unit_suite_start();
    unit_run_test(test_suffix_array);
    return NULL;
}

RUN_TESTS(all_tests);
