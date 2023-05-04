#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
  
int g = 0;
  
void *myThreadFun(void *vargp)
{
    pthread_t *arg = (pthread_t *)vargp;
    
    printf("wowow %d\n", *arg);
    printf("THANKS %d\n", *arg);
    sleep(1);

    return arg;

}
  
int main()
{
    int n = 3;
    pthread_t tid1, tid2;

    pthread_create(&tid1, NULL, myThreadFun, &tid1);
    pthread_create(&tid2, NULL, myThreadFun, &tid2);

    int *result, *result2;
 

    pthread_join(tid1, (void*)&result);
    pthread_join(tid2, (void*)&result2);
    printf("TIDS ARE: %d %d\n", *result, *result2);


    pthread_exit(NULL);
    return 0;
}