#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>



static volatile int cnt = 0;

void com(){
    printf("What a signal %d\n", cnt);
    cnt++;
}


int main() 
{
    signal(SIGUSR1, com);
    pid_t pid = fork();

    if (pid == -1) 
    {
        perror("fork");
        return(1);
    } 
    else if(pid == 0)
    {
        while(1)
        {
        sleep(2);
        }
    }
    else
    {
        for(int j = 0; j < 10; j++)
        {
            sleep(1);
            kill(pid, SIGUSR1);
        }
    }
    return 0;
}

