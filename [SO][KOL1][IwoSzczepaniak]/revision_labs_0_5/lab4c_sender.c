#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

void handler(int sig)
{
    printf("Sender: received confirmation from catcher\n");    
}

int main(int argc, char *argv[]) {
    if (argc < 2) 
    {
        printf("Usage: ./sender <catcher_pid> <mode_nr> <mode_nr> ...\n");
        return 1;
    }

    int catcher_pid = atoi(argv[1]);
    signal(SIGUSR1, handler);

    for (int i = 2; i < argc; i++) 
    {
        int mode = atoi(argv[i]);
        printf("Sender: sending SIGUSR1 signal with mode %d\n", mode);

        union sigval value;
        value.sival_int = mode;
        if (sigqueue(catcher_pid, SIGUSR1, value) == -1) 
        {
            perror("sigqueue error");
            return 1;
        }
        pause();
    }

return 0;
}