#ifndef DICT_H_C4G915WI
#define DICT_H_C4G915WI

typedef struct dict* Dict;
typedef int (*Dict_comparator)(const void* a, const void* b);
typedef unsigned long (*Dict_hash_function)(const char* key);

Dict DictCreate(Dict_comparator compare_cb, Dict_hash_function hash_function);
void DictInsert(Dict dict, void* key, void* value);
const void* DictSearch(Dict dict, const void* key);
void DictDelete(Dict dict, const void* key);
void DictDestory(Dict dict);

#endif /* end of include guard: DICT_H_C4G915WI */
