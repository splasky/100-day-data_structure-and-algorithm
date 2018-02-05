#include "../include/hashmap_algo.h"
#include <stdlib.h>
#include <string.h>

const uint32_t FNV_PRIME = 16777619;
const uint32_t FNV_OFFSET_BASIS = 2166136261;

uint32_t Hashmap_fnv1a_hash(void* data)
{
    const char* s = (char*)data;
    uint32_t hash = FNV_OFFSET_BASIS;
    const size_t str_len = strlen(s);
    for (size_t i = 0; i < str_len; ++i) {
        hash ^= *s;
        hash *= FNV_PRIME;
    }

    return hash;
}

uint32_t Hashmap_djb2_hash(void* data)
{
    uint32_t hash = 5381;
    int c = *(char*)data;

    while (c) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        c = *(char*)data++;
    }
    return hash;
}
