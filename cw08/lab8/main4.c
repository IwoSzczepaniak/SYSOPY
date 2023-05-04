#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
  
pthread_t tid1;

  
void *myThreadFun(void *vargp)
{
    while(1) sleep(1);
}

void handler1(int sig)
{
    pthread_kill(tid1, SIGUSR2);
}

void handler2(int sig)
{
    printf("dostałem sygnał");
}

  
int main()
{
    int n = 3;

    pthread_create(&tid1, NULL, myThreadFun, &tid1);

    signal(SIGUSR1, handler1);
    signal(SIGUSR2, handler2);

    sleep(1);
    raise(SIGUSR1);

    pthread_exit(NULL);
    return 0;
}