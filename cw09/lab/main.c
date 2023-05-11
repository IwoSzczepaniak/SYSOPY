#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int global;

typedef struct {
    pthread_mutex_t mtx_;
} global_t;

void *do_sth(void *arg)
{
    global_t *args = (global_t *) arg;
    pthread_mutex_t mtx = args->mtx_;
    while(global < 10)
    {
        pthread_mutex_lock(&mtx);
        global++;
        printf("Global is: %d\n", global);
        pthread_mutex_unlock(&mtx);
    }
}

int main(int argc, char *argv[]) 
{
    if (argc != 2) 
    {
        printf("Usage: %s N\n", argv[0]);
        return 1;
    }

    int n = atoi(argv[1]);
    global = 0;
    pthread_t barber_threads[n];
    pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
    for (int i = 0; i < n; i++) 
    {
        global_t *args = (global_t *) malloc(sizeof(global_t));
        args->mtx_ = mtx;
        pthread_create(&barber_threads[i], NULL, do_sth, (void *) args);
    }

    for (int i = 0; i < n; i++) pthread_join(barber_threads[i], NULL);


    return 0;
}
