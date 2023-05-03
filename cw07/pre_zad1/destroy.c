#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "shared_mem.h"



extern int main()
{
    if (destroy_memory_block(FILENAME)) printf("destroyed correctly\n");
    else printf("not destroyed\n");
    return 0;
}
