#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "shared_mem.h"

extern int main()
{
    char *block = attach_memory_block(FILENAME, BLOCK_SIZE);
    if (block == NULL) return -1;

    printf("reading %s\n", block);

    detach_memory_block(block);
    return 0;
}
