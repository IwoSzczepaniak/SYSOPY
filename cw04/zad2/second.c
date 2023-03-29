#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

char* pname = "Parent";

void handler(int signum)
{
    printf("%s process received signal %d\n", pname, signum);
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

    for (int i = 0; i < 2; i++)
    {
        pid = fork();

        if (pid == -1) 
        {
            perror("fork");
            exit(EXIT_FAILURE);
        } 
        else if (pid == 0) 
        {
            pname = "Child";
            printf("\n%s process running, do some things:\n", pname);
            for(int i = 0; i < 2; i++) {
                printf("Hi\n");
                sleep(1);
            }
            sleep(1);
            exit(EXIT_SUCCESS);
        }
        else 
        {
            printf("\n%s process waiting\n", pname);
            wait(NULL);
            printf("%s process recived information about closing child process\n", pname);
            sleep(1);
        }
    }
    return 0;
}
