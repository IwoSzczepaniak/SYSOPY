#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "shared_mem.h"

extern int main()
{
    char *block = attach_memory_block(FILENAME, BLOCK_SIZE);
    if (block == NULL) return -1;
    

    while(1)
    {
        if(strlen(block) > 0)
        {
            printf("reading %s\n", block);
            bool quit_cmd = (strcmp(block, "quit")==0);
            block[0]=0;
            if(quit_cmd) {break;}
        }
    }


    detach_memory_block(block);
    return 0;
}
