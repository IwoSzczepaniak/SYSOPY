#ifndef SHARED_MEM_LIB_H
#define SHARED_MEM_LIB_H

#include <stdbool.h>

char *attach_memory_block(char *filename, int size);
bool detach_memory_block(char *block);
bool destroy_memory_block(char *filename);

int create_semaphore(int key, int initial_value);
void semaphore_wait(int semaphore_id);
void semaphore_signal(int semaphore_id);
void delete_semaphore(int semaphore_id);

#endif