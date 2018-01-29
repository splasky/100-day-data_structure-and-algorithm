#include "../include/hashmap.h"
#include "../include/dbg.h"
#include <stdbool.h>
#include <string.h>

static int default_compare(void* a, void* b) { return strcmp(a, b); }

/* Bob Jenkins's hash algorithm */
static uint32_t default_hash(void* a)
{

    size_t len = strlen(a);
    char* key = a;
    uint32_t hash = 0;
    uint32_t i = 0;

    for (hash = i; i < len; ++i) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

Hashmap* Hashmap_create(Hashmap_compare compare, Hashmap_hash hash)
{
    Hashmap* map = calloc(1, sizeof(Hashmap));
    check_mem(map);

    map->compare = compare == NULL ? default_compare : compare;
    map->hash = hash == NULL ? default_hash : hash;
    map->buckets = Darray_create(sizeof(Darray*), DEFAULT_NUMBER_OF_BUCKETS);
    /* map->buckets->end = map->buckets->max; */
    check_mem(map->buckets);
    return map;

error:
    if (map) {
        Hashmap_destroy(map);
    }

    return NULL;
}

void Hashmap_destroy(Hashmap* map)
{

    if (map) {
        if (map->buckets) {
            for (int i = 0; i < Darray_count(map->buckets); ++i) {
                Darray* bucket = Darray_get(map->buckets, i);
                if (bucket) {
                    for (int j = 0; j < Darray_count(bucket); j++) {
                        free(Darray_get(bucket, j));
                    }
                    Darray_destroy(bucket);
                }
            }
            Darray_destroy(map->buckets);
        }
        free(map);
    }
}

static inline Hashmap_node* Hashmap_node_create(int hash, void* key, void* data)
{
    Hashmap_node* node = calloc(1, sizeof(Hashmap_node));
    check_mem(node);

    node->key = key;
    node->data = data;
    node->hash = hash;

    return node;
error:
    return NULL;
}

static inline Darray* Hashmap_find_bucket(
    Hashmap* map, void* key, _Bool create, uint32_t* hash_out)
{
    uint32_t hash = map->hash(key);
    int bucket_n = hash % DEFAULT_NUMBER_OF_BUCKETS;
    check(bucket_n >= 0, "Invalid bucket found: %d", bucket_n);
    *hash_out = hash;

    Darray* bucket = Darray_get(map->buckets, bucket_n);

    if (!bucket && create) {
        bucket = Darray_create(sizeof(void*), DEFAULT_NUMBER_OF_BUCKETS);
        check_mem(bucket);
        Darray_set(map->buckets, bucket_n, bucket);
    }
    return bucket;
error:
    return NULL;
}

int Hashmap_set(Hashmap* map, void* key, void* data)
{

    uint32_t hash = 0;
    Darray* bucket = Hashmap_find_bucket(map, key, true, &hash);
    check(bucket, "Error can't create bucket.");

    Hashmap_node* node = Hashmap_node_create(hash, key, data);
    check_mem(node);

    Darray_push(bucket, node);
    return 0;
error:
    return -1;
}

static inline int Hashmap_get_node(Hashmap* map, uint32_t hash, Darray* bucket, void* key)
{
    for (int i = 0; i < Darray_count(bucket); ++i) {
        Hashmap_node* node = Darray_get(bucket, i);
        if (node->hash == hash && map->compare(node->key, key) == 0) {
            return i;
        }
    }

    return -1;
}

void* Hashmap_get(Hashmap* map, void* key)
{

    uint32_t hash = 0;
    Darray* bucket = Hashmap_find_bucket(map, key, false, &hash);
    if (!bucket)
        return NULL;

    int i = Hashmap_get_node(map, hash, bucket, key);
    if (i == -1)
        return NULL;

    Hashmap_node* node = Darray_get(bucket, i);
    check(node != NULL, "Failed to get node from bucket when it should exist.");
    return node->data;

error:
    return NULL;
}

int Hashmap_traverse(Hashmap* map, Hashmap_traverse_cb traverse_cb)
{
    int rc = 0;

    for (int i = 0; i < Darray_count(map->buckets); ++i) {
        Darray* bucket = Darray_get(map->buckets, i);
        if (bucket) {
            for (int j = 0; j < Darray_count(bucket); j++) {
                Hashmap_node* node = Darray_get(bucket, i);
                if (node) {
                    rc = traverse_cb(node);
                    if (rc != 0) {
                        return rc;
                    }
                }
            }
        }
    }

    return 0;
}

void* Hashmap_remove(Hashmap* map, void* key)
{
    uint32_t hash = 0;
    Darray* bucket = Hashmap_find_bucket(map, key, false, &hash);

    if (!bucket)
        return NULL;

    int i = Hashmap_get_node(map, hash, bucket, key);
    /* find nothing */
    if (i == -1)
        return NULL;

    Hashmap_node* node = Darray_get(bucket, i);
    void* data = node->data;
    free(node);

    Hashmap_node* ending = Darray_pop(bucket);

    if (ending != node) {
        Darray_set(bucket, i, ending);
    }

    return data;
}

int Hashmap_length(Hashmap* map)
{
    if (map && map->buckets) {
        return Darray_count(map->buckets);
    }

    return 0;
}
