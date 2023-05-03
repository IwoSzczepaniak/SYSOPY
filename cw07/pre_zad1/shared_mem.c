#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shared_mem.h"

static int get_shared_block(char *filename, int size)
{
    key_t key;
    key = ftok(filename, 0);
    if(key == -1)
    {
        perror("key");
        return -1;
    }

    return shmget(key, size, 0666 | IPC_CREAT);
}

char *attach_memory_block(char *filename, int size)
{
    int shared_block_id = get_shared_block(filename, size);
    if (shared_block_id == -1) return NULL;

    char *result = shmat(shared_block_id, NULL, 0);
    if (result == (char *) -1) 
    {
        perror("shmat");
        return NULL;
    }

    return result;
}

bool detach_memory_block(char *block)
{
    return (shmdt(block) != -1);
}

bool destroy_memory_block(char *filename)
{
    int shared_block_id = get_shared_block(filename, 0);
    if (shared_block_id == -1) return NULL;
    
    return (shmctl(shared_block_id, IPC_RMID, NULL) != -1);
}
