#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void sighandler(int sig, siginfo_t *info, void *ucontext)
{
    printf("Signal status: %d\n", info->si_status);
    // exit(EXIT_SUCCESS);
    exit(info->si_status);
}
/*
 * Funkcja 'get_child_code' powinna poczekać na zakończenie dowolnego
 * procesu potomnego. Jeśli proces zakończył się poprawnie, powinna
 * zwrócić jego kod wyjścia, w przeciwnym wypadku -1. 
 */
int get_child_code(void) {
   
    // if (pause()) return 1;
    // else return -1;
    struct sigaction action;
    action.sa_sigaction = &sighandler;
    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGCHLD);
    wait(NULL);
    // should be sth from signal
    }

void spawn_fib(int n) {
    pid_t id = fork();
    if (id == 0) {
        char buf[20];
        snprintf(buf, sizeof(buf), "%d", n);
        execl("main", "main", buf, NULL);
    }
}

int fib(int n) {
    if (n == 0 || n == 1) {
        return 1;
    } else {
        spawn_fib(n - 1);
        spawn_fib(n - 2);
        return get_child_code() + get_child_code();
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fputs("Usage: ./main N\n", stderr);
        exit(-1);
    }
    int n = atoi(argv[1]);
    if (n > 11 || n < 0) {
        fprintf(stderr, "Argument out of range: %d\n", n);
        exit(-1);
    } else {
        return fib(n);
    }
}
