#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <unistd.h>
#include <sys/wait.h>

void sighandler(int sig, siginfo_t *info, void *ucontext)
{
    printf("Signal value: %d \n", info->si_value.sival_int);
}


int main(int argc, char* argv[]) {

    if(argc != 3){
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    struct sigaction action;
    action.sa_sigaction = &sighandler;


    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGUSR1);
    sigaddset(&action.sa_mask, SIGUSR2);

    int child = fork();
    if(child == 0) {
        sigset_t block_mask;
        sigfillset(&block_mask);
        sigdelset(&block_mask, SIGUSR1);
        sigdelset(&block_mask, SIGUSR2);
        sigprocmask(SIG_BLOCK, &block_mask, NULL);
        wait(NULL);

        //zablokuj wszystkie sygnaly za wyjatkiem SIGUSR1 i SIGUSR2
        //zdefiniuj obsluge SIGUSR1 i SIGUSR2 w taki sposob zeby proces potomny wydrukowal
        //na konsole przekazana przez rodzica wraz z sygnalami SIGUSR1 i SIGUSR2 wartosci
    }
    else {
        int val = atoi(argv[1]);
        int proc = atoi(argv[2]);
        union sigval sv;
        sv.sival_int = val;
        sigqueue(child, proc, sv);
        //wyslij do procesu potomnego sygnal przekazany jako argv[2]
        //wraz z wartoscia przekazana jako argv[1]
    }

    return 0;
}
