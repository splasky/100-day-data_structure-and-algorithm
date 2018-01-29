#ifndef HASHMAP_H_8DN6OOUV
#define HASHMAP_H_8DN6OOUV

#include "../include/darray.h"
#include <stdint.h>

typedef int (*Hashmap_compare)(void* a, void* b);
typedef uint32_t (*Hashmap_hash)(void* key);

typedef struct _hashmap {
    Darray* buckets;
    Hashmap_compare compare;
    Hashmap_hash hash;
} Hashmap;

typedef struct map_t {
    void* key;
    void* data;
    uint32_t hash;
} Hashmap_node;

#define DEFAULT_NUMBER_OF_BUCKETS 100

Hashmap* Hashmap_create(Hashmap_compare compare, Hashmap_hash);
void Hashmap_destroy(Hashmap* map);

int Hashmap_set(Hashmap* map, void* key, void* data);
void* Hashmap_get(Hashmap* map, void* key);

typedef int (*Hashmap_traverse_cb)(Hashmap_node* node);
int Hashmap_traverse(Hashmap* map, Hashmap_traverse_cb traverse_cb);

void* Hashmap_remove(Hashmap* map, void* key);
int Hashmap_length(Hashmap* map);
#endif /* end of include guard: HASHMAP_H_8DN6OOUV */
