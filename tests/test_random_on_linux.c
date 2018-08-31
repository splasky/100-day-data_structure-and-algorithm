#include "unit.h"
#include <stdio.h>

TEST(test_random)
{
    unsigned int rand_num = 0x0;
    FILE* f;
    f = fopen("/dev/random", "r");
    size_t ret = fread(&rand_num, sizeof(rand_num), 1, f);
    fclose(f);

    if (ret == sizeof(rand_num))
    {
        printf("%u\n", rand_num);
    }

    return NULL;
}

RUN_TESTS(test_random);
