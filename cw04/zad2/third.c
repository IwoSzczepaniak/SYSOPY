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
    sa.sa_flags = SA_RESETHAND;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGINT, &sa, NULL);

    printf("Sending SIGINT:\n");
    raise(SIGINT);

    printf("Sending SIGINT again:\n");
    raise(SIGINT);

    

    return 0;
}
