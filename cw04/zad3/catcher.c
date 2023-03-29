#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

int mode;
int count = 0;

void sigusr1_handler(int signum, siginfo_t *info, void *context) 
{
    printf("Catcher %d: received SIGUSR1 signal from Sender %d\n", getpid(), info->si_pid);
    int mode = info->si_value.sival_int;
    count++;
    kill(info->si_pid, SIGUSR1); 

    if (mode == 1)
    {
        for (int i = 1; i <= 100; i++) printf("%d ", i);
        printf("\n");
    }
    else if (mode == 2)
    {
        time_t rawtime;
        time(&rawtime);
        printf("Current time: %s", ctime(&rawtime));
    } 
    else if (mode == 3) 
    {
        printf("Number of mode change requests: %d\n", count);
    } 
    else if (mode == 4) 
    {
        while (mode == 4) 
        {
            time_t rawtime;
            time(&rawtime);
            printf("Current time: %s", ctime(&rawtime));
            sleep(1);
        }
    }
    else if (mode == 5)
    {
        printf("Catcher: received exit mode signal, terminating...\n");
        exit(0);
    }
    
}

int main() 
{
    printf("Catcher PID: %d\n", getpid());

    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO | SA_NODEFER;
    sa.sa_sigaction = sigusr1_handler;
    sigemptyset(&sa.sa_mask);
    sigaddset(&sa.sa_mask, SIGUSR1);
    sigaction(SIGUSR1, &sa, NULL);

    while (mode != 5) 
    {
        pause();
    }

    return 0;
}
