#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
  
int g = 0;
  
void *myThreadFun(void *vargp)
{
    pthread_t *arg = (pthread_t *)vargp;

    int i = 0;
    pthread_t tid = pthread_self();
    while(1) 
    {
        printf("Thread ID: %d, Iteration: %d, Global: %d\n", tid, i++, g++);
        sleep(2);
        if(arg != NULL) pthread_cancel(*arg);
    }
       
}
  
int main()
{
    int n = 3;
    pthread_t tid1, tid2;


    pthread_create(&tid1, NULL, myThreadFun, NULL);
    pthread_create(&tid2, NULL, myThreadFun, &tid1);

   

    

    pthread_exit(NULL);
    return 0;
}