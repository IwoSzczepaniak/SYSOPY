#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
  
int g = 0;
  
void *myThreadFun(void *vargp)
{
    // Store the value argument passed to this thread
    int *arg = (int *)vargp;

    int i = 0;
    pthread_t tid = pthread_self();
    while(1) 
    {
        printf("Thread ID: %d, Argument: %d, Iteration: %d, Global: %d\n", tid, *arg, i++, g++);
        sleep(2);
    }
}
  
int main()
{
    for (int i = 0; i < 3; i++)
    {    
        pthread_t tid;
        pthread_create(&tid, NULL, myThreadFun, &i);
    }

    pthread_exit(NULL);
    return 0;
}