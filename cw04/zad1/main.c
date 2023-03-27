#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

sigset_t mask;

void handler(int sig)
{
    printf("Received signal %d\n", sig);
}


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Type one of the following: <ignore, handler, mask or pending>\n");
        exit(1);
    }

    if (strcmp(argv[1], "ignore") == 0)
    {
        signal(SIGUSR1, SIG_IGN);
    }

    else if (strcmp(argv[1], "handler") == 0)
    {
        signal(SIGUSR1, handler);
    }

    else if (strcmp(argv[1], "mask") == 0)
    {
        signal(SIGUSR1, handler);
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
        {
            perror("Signal was not blocked\n");
            exit(1);
        }
        else
        {
            printf("Signal was blocked\n");
        }
    }

    else if (strcmp(argv[1], "pending") == 0)
    {
        signal(SIGUSR1, handler);
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        if (sigpending(&mask) == -1) perror("sigpending");
        if (sigismember(&mask, SIGUSR1) == 1)
        {
            printf("SIGUSR1 is pending\n");
        }
        else
        {
            printf("SIGUSR1 is not pending\n");
        }
    }
    
    raise(SIGUSR1);
    sleep(1);

    int pid = fork();
    if (pid == -1) perror("fork");

    if (pid == 0)
    {
        printf("Child process\n");
        sleep(1);
        if (strcmp(argv[1], "pending") == 0)
        {   
            if (sigpending(&mask) == -1) perror("sigpending");
            if (sigismember(&mask, SIGUSR1) == 1)
            {
                printf("SIGUSR1 is pending\n");
            }
            else
            {
                printf("SIGUSR1 is not pending\n");
            }
        }
        else
        {
        raise(SIGUSR1);
        }
    }  
    
    else
    {
        sleep(2);
    }

    return 0;
}
