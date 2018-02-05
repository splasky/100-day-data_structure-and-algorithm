#undef NDEBUG
#ifndef _UNIT_TEST_H_
#define _UNIT_TEST_H_

#include "../include/dbg.h"
#include <stdio.h>
#include <stdlib.h>

#define TEST(name) char* name(void)

#define unit_suite_start() char* message = NULL

#define unit_assert(test, message)                                                       \
    if (!(test)) {                                                                       \
        log_err(message);                                                                \
        return message;                                                                  \
    }
#define unit_run_test(test)                                                              \
    debug("\n-----%s", " " #test);                                                       \
    message = test();                                                                    \
    tests_run++;                                                                         \
    if (message)                                                                         \
        return message;

#define RUN_TESTS(name)                                                                  \
    int main(int argc, char* argv[])                                                     \
    {                                                                                    \
        if (argc < 1) {                                                                  \
            log_err("No input tests.");                                                  \
            exit(1);                                                                     \
        }                                                                                \
        argc = 1;                                                                        \
        debug("RUNNING: %s", argv[0]);                                                   \
        printf(C_MAGENTA "----RUNNING: %s\n" C_NORMAL, argv[0]);                         \
        char* result = name();                                                           \
        if (result != 0) {                                                               \
            printf(C_RED "FAILED: %s\n" C_NORMAL, result);                               \
        } else {                                                                         \
            printf(C_GREEN "ALL TESTS PASSED\n" C_NORMAL);                               \
        }                                                                                \
        printf(C_CYAN "Tests run: %d\n" C_NORMAL, tests_run);                            \
        exit(result != 0);                                                               \
    }

int tests_run;

#endif /* ifndef */
