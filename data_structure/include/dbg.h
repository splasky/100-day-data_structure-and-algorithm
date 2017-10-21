#ifndef __dbg_h__
#define __dbg_h__

#include <errno.h>
#include <stdio.h>
#include <string.h>

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...)                                                                    \
    fprintf(stderr, C_GRAY "[DEBUG] %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define C_NORMAL "\033[0m"
#define C_RED "\033[1;31m"
#define C_GREEN "\033[1;32m"
#define C_YELLOW "\033[1;33m"
#define C_BULE "\033[1;34m"
#define C_MAGENTA "\033[1;35m"
#define C_CYAN "\033[1;36m"
#define C_GRAY "\033[1;90m"

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...)                                                                  \
    fprintf(stderr, C_RED "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__,      \
        clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...)                                                                 \
    fprintf(stderr, C_YELLOW "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__,    \
        clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...)                                                                 \
    fprintf(stderr, C_NORMAL "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define check(A, M, ...)                                                                 \
    if (!(A)) {                                                                          \
        log_err(M, ##__VA_ARGS__);                                                       \
        errno = 0;                                                                       \
        goto error;                                                                      \
    }

#define sentinel(M, ...)                                                                 \
    {                                                                                    \
        log_err(M, ##__VA_ARGS__);                                                       \
        errno = 0;                                                                       \
        goto error;                                                                      \
    }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, M, ...)                                                           \
    if (!(A)) {                                                                          \
        debug(M, ##__VA_ARGS__);                                                         \
        errno = 0;                                                                       \
        goto error;                                                                      \
    }

#endif
