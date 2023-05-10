#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>


void handler(int signum, siginfo_t* info, void* context)
{
    printf("Received signal %d from process %d\n", signum, info->si_pid);
    printf("Signal code: %d\n", info->si_code);
    printf("Signal value: %d\n", info->si_value.sival_int);
    printf("User id: %d\n", info->si_uid);
}

int main() {
    char *str_id;
    struct sigaction sa;
    sa.sa_sigaction = &handler;
    sa.sa_flags = SA_SIGINFO;
    
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }


    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    raise(SIGTERM);
    raise(SIGINT);
    raise(SIGUSR1);
    return 0;
}
