#include "../include/util.h"
#include "../include/dbg.h"
#include <stdint.h>
#include <stdlib.h>

char* open_file(char* file)
{
    debug("file:%s", file);
    FILE* fp;
    if (!(fp = fopen(file, "r")))
    {
        printf("Cannot open file\n");
        return NULL;
    }

    uint64_t lsize = 0;
    char* buffer;
    size_t result;
    // obtain file size
    fseek(fp, 0, SEEK_END);
    lsize = ftell(fp);
    rewind(fp);

    // allocate memory to contain the while file
    if (!(buffer = malloc(sizeof(char) * lsize)))
    {
        printf("Allocate memory area for buffer error!\n");
        goto error;
    }

    result = fread(buffer, 1, lsize, fp);
    if (result != lsize)
    {
        printf("Read file error\n");
        free(buffer);
        goto error;
    }
    fclose(fp);
    return buffer;
error:
    fclose(fp);
    return NULL;
}
