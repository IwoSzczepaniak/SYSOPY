#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

sigset_t mask, oldmask;

void handler(int sig)
{
    printf("Handler received signal: %d\n", sig);
}

void check(char* argv[], char* pname){
    if (strcmp(argv[1], "mask") == 0)
    {
        if(sigismember(&mask, SIGUSR1) == 1)
        {
            printf("Signal was blocked - %s\n", pname);
        }
        else
        {
            printf("Signal was not blocked - %s\n", pname);
        }

    }

    else if (strcmp(argv[1], "pending") == 0)
    {
        if (sigpending(&mask) < 0)
        {
            printf("Error in sigpending\n");
            exit(1);
        }
        else
        {
            if(sigismember(&mask, SIGUSR1) == 1)
            {
                printf("Signal is pending - %s\n", pname);
            }
            else
            {
                printf("Signal is not pending - %s\n", pname);
            }
        }
    }
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
        // signal(SIGUSR1, handler);
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
        {
            printf("Error in sigprocmask\n");
            exit(1);
        }

    }

    else if (strcmp(argv[1], "pending") == 0)
    {
        sigemptyset (&mask);
        sigaddset (&mask, SIGUSR1);
        if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
        {
            printf("Error in sigprocmask\n");
            exit(1);
        }
    }
    
    raise(SIGUSR1);
    check(argv, "parent");


    int pid = fork();
    if (pid == -1) perror("fork");

    if (pid == 0)
    {
        if (strcmp(argv[1], "ignore") != 0)printf("Now output from child process:\n");

        if (strcmp(argv[1], "pending") != 0) raise(SIGUSR1);

        check(argv, "child");
    }
    
    else
    {
        sleep(1);
    }

    return 0;
}
