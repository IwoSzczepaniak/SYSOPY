#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define NUM_REINDEER 9
#define NUM_ELVES 10

pthread_mutex_t santa_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t reindeer_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t elves_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t santa_cond = PTHREAD_COND_INITIALIZER;

int reindeer_arrived = 0;
int elves_waiting = 0;
int elves_handled = 0;

void *santa_thread(void *arg);
void *reindeer_thread(void *arg);
void *elf_thread(void *arg);

int main()
{
    srand(time(NULL));
    
    pthread_t santa_tid, reindeer_tids[NUM_REINDEER], elf_tids[NUM_ELVES];
    
    pthread_create(&santa_tid, NULL, santa_thread, NULL);
    
    for (int i = 0; i < NUM_REINDEER; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&reindeer_tids[i], NULL, reindeer_thread, (void*)id);
    }
    
    for (int i = 0; i < NUM_ELVES; i++) {
        int* id = malloc(sizeof(int));
        *id = i + 1;
        pthread_create(&elf_tids[i], NULL, elf_thread, (void*)id);
    }
    
    for (int i = 0; i < NUM_REINDEER; i++) pthread_join(reindeer_tids[i], NULL);
    for (int i = 0; i < NUM_ELVES; i++) pthread_join(elf_tids[i], NULL);
    pthread_join(santa_tid, NULL);
    
    return 0;
}


void* santa_thread(void* arg)
{
    while (!(reindeer_arrived == NUM_REINDEER && elves_handled == NUM_ELVES)) {
        pthread_mutex_lock(&santa_mutex);
        // printf("Niebsłużone elfy %d oraz nieobsłużone renifery %d\n", NUM_ELVES-elves_handled, NUM_REINDEER-reindeer_arrived);
        while (!(reindeer_arrived >= NUM_REINDEER || (elves_waiting >= 3 && elves_handled < NUM_ELVES))) {
            printf("Mikołaj: zasypiam\n");
            pthread_cond_wait(&santa_cond, &santa_mutex);
        }
        
        if (reindeer_arrived == NUM_REINDEER) {
            printf("Mikołaj: budzę się\n");
            printf("Mikołaj: dostarczam zabawki\n");
            sleep(rand() % 3 + 2);
            reindeer_arrived = 0;
            printf("Mikołaj: dostarczyłem zabawki\n");
            pthread_cond_broadcast(&reindeer_cond);
        } 
        
        else if (elves_waiting == 3 && elves_handled < NUM_ELVES) {
            printf("Mikołaj: budzę się\n");
            printf("Mikołaj: rozwiązuję problemy elfów\n");
            sleep(rand() % 2 + 1);
            elves_waiting -= 3;
            pthread_cond_broadcast(&elves_cond);
        }
        
        pthread_mutex_unlock(&santa_mutex);
    }

    printf("Mikołaj: wykonałem wszystkie moje zadania na ten rok! Dobranoc!\n");
    pthread_exit(NULL);
}


void* reindeer_thread(void* arg)
{
    int id = *((int*)arg);
    sleep(rand() % 6 + 5);

    pthread_mutex_lock(&santa_mutex);
    printf("Renifer: czeka %d reniferów na Mikołaja, ID: %d\n", ++reindeer_arrived, id);
    
    if (reindeer_arrived == NUM_REINDEER) 
    {
        printf("Renifer: wybudzam Mikołaja, ID: %d\n", id);
        pthread_cond_signal(&santa_cond);
    }
    
    pthread_mutex_unlock(&santa_mutex);
    
    pthread_exit(NULL);
}


void* elf_thread(void* arg)
{
    int id = *((int*)arg);
    int has_problem = 0;
    int self_solved = 0;

    while (!has_problem) {   
        sleep(rand() % 4 + 2);   
        pthread_mutex_lock(&santa_mutex);

        if (elves_waiting < 3) 
        {
            printf("Elf: czeka %d elfów na Mikołaja, ID: %d\n", ++elves_waiting, id);
            
            if (elves_waiting == 3) 
            {
                printf("Elf: wybudzam Mikołaja, ID: %d\n", id);
                pthread_cond_signal(&santa_cond);
            }
            
            pthread_mutex_unlock(&santa_mutex);
            pthread_cond_wait(&elves_cond, &santa_mutex);
            has_problem = 1;
        } 
        else
        {
            printf("Elf: samodzielnie rozwiązuję swój problem, ID: %d\n", id);
            has_problem = 1;
            self_solved = 1;
            sleep(rand() % 2 + 1);
        }
        
        pthread_mutex_unlock(&santa_mutex);
    }
    
    if (!self_solved)
    {
     
        pthread_mutex_lock(&santa_mutex);
        printf("Elf: Mikołaj rozwiązał problem, ID: %d\n", id);
        sleep(rand() % 2 + 1);
        
        pthread_mutex_unlock(&santa_mutex);
    }   

    if (elves_handled++ == NUM_ELVES) 
    {
        printf("Elf: wszystkie problemy elfów rozwiązane, ID: %d\n", id);
        pthread_cond_broadcast(&elves_cond);
    }   
    pthread_exit(NULL);
}