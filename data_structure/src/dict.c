#include "../include/dict.h"
#include "../include/dbg.h"
#include <stdio.h>
#include <stdlib.h>

#define INITIAL_SIZE (1024)
#define GROWTH_FACTOR (2)
#define MAX_LOAD_FACTOR (1)
#define MULTIPLIER (97)

struct elt {
    struct elt* next;
    void* key;
    void* value;
};

struct dict {
    int size;
    int n;
    struct elt** table;
    Dict_comparator comparator;
    Dict_hash_function hash_function;
};

static unsigned long default_hash_function(const char* key);
static void grow(Dict dict);
static Dict internalDictCreate(
    int size, Dict_comparator comparator_cb, Dict_hash_function hash_function);

Dict DictCreate(Dict_comparator comparator_cb, Dict_hash_function hash_function)
{
    return internalDictCreate(INITIAL_SIZE, comparator_cb, hash_function);
}

void DictDestory(Dict d)
{
    struct elt* curr;
    struct elt* next;

    for (int i = 0; i < d->size; i++) {
        for (curr = d->table[i]; curr != NULL; curr = next) {
            next = curr->next;

            free(curr->key);
            curr->key = NULL;
            free(curr->value);
            curr->value = NULL;
            free(curr);
            curr = NULL;
        }
    }

    free(d->table);
    d->table = NULL;
    free(d);
    d = NULL;
}

void DictInsert(Dict dict, void* key, void* value)
{

    struct elt* ele;
    unsigned long h;

    check_mem(key);
    check_mem(value);

    ele = malloc(sizeof(struct elt));
    check_mem(ele);

    ele->key = key;
    ele->value = value;

    h = dict->hash_function(key) % dict->size;
    ele->next = dict->table[h];
    dict->table[h] = ele;
    dict->n++;

    if (dict->n >= dict->size * MAX_LOAD_FACTOR) {
        grow(dict);
    }

error:
    log_err("Insert into dict failed");
}

const void* DictSearch(Dict dict, const void* key)
{
    struct elt* curr;

    for (curr = dict->table[dict->hash_function(key) % dict->size]; curr != 0;
         curr = curr->next) {
        if (dict->comparator(curr->key, key) == 0) {
            return curr->value;
        }
    }

    return 0;
}

void DictDelete(Dict dict, const void* key)
{
    struct elt** prev;
    struct elt* freed;

    for (prev = &(dict->table[dict->hash_function(key) % dict->size]); *prev != 0;
         prev = &((*prev)->next)) {

        if (!dict->comparator((*prev)->key, key)) {
            freed = *prev;
            *prev = freed->next;

            free(freed->key);
            free(freed->value);
            free(freed);
            return;
        }
    }
}

static unsigned long default_hash_function(const char* key)
{
    unsigned const char* us;
    unsigned long h;
    h = 0;

    for (us = (unsigned const char*)key; *us; us++) {
        h = h * MULTIPLIER + *us;
    }

    return h;
}

static void grow(Dict dict)
{
    Dict d2;
    struct dict* swap;

    d2 = internalDictCreate(
        dict->size * GROWTH_FACTOR, dict->comparator, dict->hash_function);

    for (int i = 0; i < dict->n; i++) {
        for (struct elt* ele = dict->table[i]; ele != 0; ele = ele->next) {
            DictInsert(d2, ele->key, ele->value);
        }
    }

    swap = dict;
    dict = d2;
    d2 = swap;

    DictDestory(d2);
}

static Dict internalDictCreate(
    int size, Dict_comparator comparator_cb, Dict_hash_function hash_function)
{
    Dict d = malloc(sizeof(Dict));
    check_mem(d);

    d->size = size;
    d->n = 0;
    d->table = malloc(sizeof(struct elt*) * d->size);
    check_mem(d->table);
    d->comparator = comparator_cb;
    d->hash_function = (hash_function) ? hash_function : default_hash_function;

    for (int i = 0; i < d->size; i++) {
        d->table[i] = 0;
    }

    return d;
error:
    return NULL;
}
