#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int id;
key_t key;

int main(int argc, char* argv[])
{
    key = ftok(".", 'p');
    
    id = semget(key, 1, IPC_CREAT|0666);
    if(id == -1) perror("create error\n");

    if (semctl(id, 0, SETVAL, 1) == -1) perror("seting value");
    
    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;
    sb.sem_flg = 0;

    semop(id, &sb, 1);

    struct semid_ds ds;
    if(semctl(id, 0, IPC_STAT, &ds) == -1) perror("semctl");

    printf("Semafor amount %lu\n", ds.sem_nsems);
    sb.sem_op = 1;
    semop(id, &sb, 1);

    return 0;   
}

    