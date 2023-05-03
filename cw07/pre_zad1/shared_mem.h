#ifndef SHARED_MEM_LIB_H
#define SHARED_MEM_LIB_H


#include <stdbool.h>

char *attach_memory_block(char *filename, int size);
bool detach_memory_block(char *block);
bool destroy_memory_block(char *filename);

#endif