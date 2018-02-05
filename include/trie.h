#ifndef TRIE_H_JZBDVPTW
#define TRIE_H_JZBDVPTW

typedef struct trie_node Trie;
#define EMPTY_TRIE (0)

int trie_contains(Trie* trie, const char* target);
Trie* trie_insert(Trie* trie, const char* key);
void trie_destroy(Trie* trie);
void trie_print(Trie* trie);

#endif /* end of include guard: TRIE_H_JZBDVPTW */
