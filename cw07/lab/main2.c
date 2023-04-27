#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>

int id;
key_t key;
char* shared_mem;

int main(int argc, char* argv[])
{
    key = ftok(".", 'p');
    id = shmget(key, sizeof(char)*20, IPC_CREAT|0666);

    shared_mem = shmat(id, NULL, 0);
    strcpy(shared_mem, "cała na przód");
    shmdt(shared_mem);

    int pid = fork();
    if(pid == 0)
    {
        char* buff;
        buff = shmat(id, NULL, 0);
        printf("Message %s\n", buff);
        shmdt(buff);
    }
    

    return 0;   
}

    