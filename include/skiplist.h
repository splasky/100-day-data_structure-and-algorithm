#ifndef SKIPLIST_H_QZOLA5OA
#define SKIPLIST_H_QZOLA5OA

typedef struct skiplist Skiplist;

Skiplist* skiplist_create(void);
void skiplist_destroy(Skiplist* splist);
int skiplist_search(Skiplist* splist, int key);
void skiplistInsert(Skiplist* splist, int key);
void skiplistDelete(Skiplist* splist, int key);

#endif /* end of include guard: SKIPLIST_H_QZOLA5OA */
