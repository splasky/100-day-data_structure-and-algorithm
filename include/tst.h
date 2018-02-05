#ifndef TST_H_FSHSU9OJ
#define TST_H_FSHSU9OJ

typedef struct tst_node TST;
#define EMPTY_TST (0)

int tst_contains(TST* t, const char* target);
TST* tst_insert(TST* t, const char* key);
void tst_destroy(TST* t);

#endif /* end of include guard: TST_H_FSHSU9OJ */
