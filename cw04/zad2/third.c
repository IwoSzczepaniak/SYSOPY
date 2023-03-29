#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

void handler(int signum)
{
    printf("Received signal %d\n", signum);
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

   
    printf("Waiting for signal...\n");
    kill(getpid(), SIGINT);
    sleep(1);
    

    return 0;
}
