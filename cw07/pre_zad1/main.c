#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "shared_mem.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("only one argument required\n");
        return 1;
    } 



    char *block = attach_memory_block(FILENAME, BLOCK_SIZE);
    if (block == NULL) return -1;
    for(int i = 0; i < QUEUE_SIZE; i++)
    {
        printf("Writing: siemano%s\n", argv[1]);
        strncpy(block, argv[1], BLOCK_SIZE);
    }
    detach_memory_block(block);
    return 0;
}
