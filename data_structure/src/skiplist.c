#include "../include/skiplist.h"
#include "../include/dbg.h"
#include <limits.h>
#include <stdlib.h>

#define MAX_HEIGHT (32)

struct skiplist {
    int key;
    int height;
    struct skiplist* next[1];
};

static int chooseHeight(void)
{
    int i = 1;
    while (i < MAX_HEIGHT && rand() % 2 == 0)
        i++;

    return i;
}

static Skiplist* skiplistCreateNode(int key, int height)
{
    Skiplist* s;
    check(height > 0, "height must more than 0");
    check(height <= MAX_HEIGHT, "height must less than max height");
    s = malloc(sizeof(struct skiplist) + sizeof(struct skiplist*) * (height - 1));

    check_mem(s);
    s->key = key;
    s->height = height;

    return s;
error:
    return NULL;
}

Skiplist* skiplist_create(void)
{
    Skiplist* splist = skiplistCreateNode(INT_MIN, MAX_HEIGHT);
    splist->height = 1;

    for (int i = 0; i < MAX_HEIGHT; i++) {
        splist->next[i] = 0;
    }

    return splist;
}

void skiplist_destroy(Skiplist* splist)
{
    Skiplist* next;
    while (splist) {
        next = splist->next[0];
        free(splist);
        splist = next;
    }
}

int skiplist_search(Skiplist* splist, int key)
{
    for (int level = splist->height - 1; level >= 0; level--) {
        while (splist->next[level] && splist->next[level]->key <= key) {
            splist = splist->next[level];
        }
    }
    return splist->key;
}

void skiplistInsert(Skiplist* splist, int key)
{
    int level = 0;
    Skiplist* elt;
    elt = skiplistCreateNode(key, chooseHeight());

    check_mem(elt);

    if (elt->height > splist->height) {
        splist->height = elt->height;
    }

    /* search through levels taller than elt */
    for (level = splist->height - 1; level >= elt->height; level--) {
        while (splist->next[level] && splist->next[level]->key < key) {
            splist = splist->next[level];
        }
    }

    /* noew level is elt->height-1,we can start inserting */
    for (; level >= 0; level--) {
        while (splist->next[level] && splist->next[level]->key < key) {
            splist = splist->next[level];
        }

        elt->next[level] = splist->next[level];
        splist->next[level] = elt;
    }

    return;
error:
    log_err("Skiplist insert failed");
}

void skiplistDelete(Skiplist* splist, int key)
{
    int level = 0;
    Skiplist* target;

    target = splist;

    /* find left most instance of key */
    for (level = splist->height - 1; level >= 0; level--) {
        while (target->next[level] && target->next[level]->key < key) {
            target = target->next[level];
        }
    }

    target = target->next[0];

    if (target == 0 || target->key != key) {
        return;
    }

    for (level = splist->height - 1; level >= 0; level--) {
        while (splist->next[level] && splist->next[level]->key < key) {
            splist = splist->next[level];
        }

        if (splist->next[level] == target) {
            splist->next[level] = target->next[level];
        }
    }

    free(target);
}
