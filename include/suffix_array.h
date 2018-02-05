#ifndef SUFFIX_ARRAY_H_4OAYY5JL
#define SUFFIX_ARRAY_H_4OAYY5JL

#include <stdlib.h>
typedef struct suffixArray SuffixArray;

struct suffixArray {
    size_t n;
    const char* string;
    const char** suffix;
};

SuffixArray* suffixArrayCreate(const char* string);
void suffixArrayDestroy(SuffixArray* suffix_array);
size_t suffixArraySearch(SuffixArray* suffix_array, const char* substring, size_t* first);
char* suffixArrayBWT(SuffixArray* suffix_array);
char* inverseBWT(size_t len, const char* string);

#endif /* end of include guard: SUFFIX_ARRAY_H_4OAYY5JL */
