#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

sigset_t mask;
char* proc_name = "parent";


void handler(int sig)
{
    printf("Handler received signal: %d\n", sig);
}


int check(char* argv[], char* pname){
    if (strcmp(argv[1], "ignore") == 0)
    {
        printf("Signal is being send and ignored\n");
    }

    
    else if (strcmp(argv[1], "mask") == 0)
    {
        if(sigismember(&mask, SIGUSR1) == 1) printf("Signal was blocked - %s\n", pname);
        else printf("Signal was not blocked - %s\n", pname);
    }

    else if (strcmp(argv[1], "pending") == 0)
    {
        if (sigpending(&mask) < 0)
        {
            printf("Error in sigpending\n");
            return 1;
        }
        else
        {
            if(sigismember(&mask, SIGUSR1) == 1) printf("Signal is pending - %s\n", pname);
            else printf("Signal is not pending - %s\n", pname);
        }
    }
    return 0;
}


int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Type one of the following: \n<ignore, handler, mask or pending> <fork>\n<ignore, handler, mask or pending> <exec>\n");
        return 1;
    }

    if (strcmp(argv[1], "handler") == 0) signal(SIGUSR1, handler);

    else if (strcmp(argv[1], "ignore") == 0) signal(SIGUSR1, SIG_IGN);

    else if (strcmp(argv[1], "mask") == 0)
    {
        sigemptyset(&mask);
        sigaddset(&mask, SIGUSR1);
        if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
        {
            printf("Error in sigprocmask\n");
            return 1;
        }
    }

    else if (strcmp(argv[1], "pending") == 0)
    {
        sigemptyset (&mask);
        sigaddset (&mask, SIGUSR1);
        if (sigprocmask(SIG_BLOCK, &mask, NULL) < 0)
        {
            printf("Error in sigprocmask\n");
            return 1;
        }
    }
    
    raise(SIGUSR1);
    if (check(argv, proc_name) == 1) perror("check parent"); 

    if (strcmp(argv[2], "fork") == 0)
    {
        int pid = fork();
        if (pid == -1) perror("fork");

        if (pid == 0)
        {
            proc_name = "child";
            if (strcmp(argv[1], "ignore") != 0) printf("Now output from child process:\n");

            if (strcmp(argv[1], "pending") != 0) raise(SIGUSR1);

            if (check(argv, proc_name)==1) perror("check child");
        }
        
        else
        {
            sleep(1);
        }
    }

    else if (strcmp(argv[2], "exec") == 0){
        if (strcmp(argv[1], "ignore") != 0) printf("Now output from exec process:\n");
        char* exec_arg[argc];
        for(int i=0; argc-1>i;i++){
            exec_arg[i] = argv[i];
        }
        exec_arg[argc-1] = "exec_product";
        exec_arg[argc] = NULL;
        execv("./main", exec_arg);
        perror("exec");
        return 1;
    }

    return 0;
}
