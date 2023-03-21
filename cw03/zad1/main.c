#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    if (argc != 2) 
    {
        fprintf(stderr, "Usage: <num_processes>\n");
        exit(1);
    }

    int num_processes = atoi(argv[1]);

    for (int i = 0; i < num_processes; i++) 
    {
        pid_t pid = fork();

        if (pid == -1) 
        {
            perror("fork");
            exit(1);
        } 
        else if (pid == 0) 
        {
            printf("Parent process ID: %d, child process ID: %d\n", getppid(), getpid());
            exit(0);
        }
    }

   
    int status;
    wait(&status);
    printf("Argv[1] is: %s\n", argv[1]);

    return 0;
}
