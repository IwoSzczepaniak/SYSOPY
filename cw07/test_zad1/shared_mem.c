#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#include <errno.h>
#include <pthread.h>

#include "shared_mem.h"

static int get_shared_block(char *filename, int size)
{
    key_t key = ftok(filename, 0);
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


/* Create a semaphore with the specified key and initial value */
int create_semaphore(int key, int initial_value) {
    int semaphore_id = semget(key, 1, IPC_CREAT | 0666);
    if (semaphore_id == -1) {
        perror("Error creating semaphore");
        exit(1);
    }

    union semun {
        int val;
        struct semid_ds *buf;
        unsigned short *array;
    } argument;
    argument.val = initial_value;

    int result = semctl(semaphore_id, 0, SETVAL, argument);
    if (result == -1) {
        perror("Error setting semaphore value");
        exit(1);
    }

    return semaphore_id;
}

/* Wait on the semaphore, decrementing its value by 1 */
void semaphore_wait(int semaphore_id) {
    struct sembuf operation;
    operation.sem_num = 0;
    operation.sem_op = -1;
    operation.sem_flg = 0;

    int result = semop(semaphore_id, &operation, 1);
    if (result == -1) {
        perror("Error waiting on semaphore");
        exit(1);
    }
}

/* Signal the semaphore, incrementing its value by 1 */
void semaphore_signal(int semaphore_id) {
    struct sembuf operation;
    operation.sem_num = 0;
    operation.sem_op = 1;
    operation.sem_flg = 0;

    int result = semop(semaphore_id, &operation, 1);
    if (result == -1) {
        perror("Error signaling semaphore");
        exit(1);
    }
}

/* Delete the semaphore */
void delete_semaphore(int semaphore_id) {
    int result = semctl(semaphore_id, 0, IPC_RMID, 0);
    if (result == -1) {
        perror("Error deleting semaphore");
        exit(1);
    }
}