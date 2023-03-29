#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void handler(int signum)
{
    printf("Received signal %d\n", signum);
}

int main() {
    pid_t pid;
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = SA_NOCLDSTOP;

    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 3; i++)
    {
        pid = fork();

        if (pid == -1) 
        {
            perror("fork");
            exit(EXIT_FAILURE);
        } 
        else if (pid == 0) 
        {
            printf("Child process running, do some things:\n");
            for(int i = 0; i < 5; i++) {
                printf("Child process: %d\n", i);
                sleep(1);
            }
            sleep(1);
            exit(EXIT_SUCCESS);
        }
        else 
        {
            printf("Parent process waiting\n");
            pause();
        }
    }
    return 0;
}
