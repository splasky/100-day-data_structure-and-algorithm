#include "../include/tst.h"
#include "../include/dbg.h"

#include <stdio.h>
#include <stdlib.h>

struct tst_node
{
    char key;
    struct tst_node* lt;
    struct tst_node* eq;
    struct tst_node* gt;
};

int tst_contains(TST* t, const char* target)
{
    check(target, "Target is NULL");

    while (t)
    {
        if (*target < t->key)
        {
            t = t->lt;
        }
        else if (*target > t->key)
        {
            t = t->gt;
        }
        else if (*target == '\0')
        {
            return 1;
        }
        else
        {
            t = t->eq;
            target++;
        }
    }

error:
    return 0;
}

static void tst_insert_iterative(TST** t, const char* key)
{
    for (;;)
    {
        if (*t == 0)
        {
            *t = malloc(sizeof(**t));
            check_mem(*t);
            (*t)->key = *key;
            (*t)->lt = (*t)->eq = (*t)->gt = 0;
        }

        /* now follow search */
        if (*key < (*t)->key)
        {
            t = &(*t)->lt;
        }
        else if (*key > (*t)->key)
        {
            t = &(*t)->gt;
        }
        else if (*key == '\0')
        {
            /* do nothing, we are done */
            return;
        }
        else
        {
            t = &(*t)->eq;
            key++;
        }
    }
error:
    return;
}

/* add a new key to a TST */
/* and return the new TST */
TST* tst_insert(TST* t, const char* key)
{
    check(key, "Key is NULL");

#ifdef USE_RECURSIVE_INSERT
    tst_insert_recursive(&t, key);
#else
    tst_insert_iterative(&t, key);
#endif
    return t;
error:
    return NULL;
}

/* free a TST */
void tst_destroy(TST* t)
{
    if (t)
    {
        tst_destroy(t->lt);
        tst_destroy(t->eq);
        tst_destroy(t->gt);
        free(t);
    }
}
