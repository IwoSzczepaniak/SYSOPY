#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/sem.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>

#include "shared_mem.h"

#define MAX_CLIENTS 20
#define MAX_CUTTING_TIME 2
#define MAX_BARBERS 10

typedef struct{
    int barber_semaphores[MAX_BARBERS];
    int customer_semaphores[MAX_CLIENTS];
    int num_barbers;
    int num_chairs;
    int queue_size;
    int num_customers;
    int waiting;
    int num_clients;
} Salon;

typedef struct {
    Salon *salon;
    int id;
} BarberThreadArgs;

typedef struct {
    Salon *salon;
    int id;
} CustomerThreadArgs;

bool barber_free[MAX_BARBERS];

int seek_for_barber(CustomerThreadArgs *args)
{
    for (int i = 0; i < args->salon->num_barbers; i++)
    {
        if (barber_free[i] == true) return i;
    }
    return -1;
}

void* barber_thread(void* arg) {
    BarberThreadArgs *args = (BarberThreadArgs *) arg;
    while (1) {
        // Check if there is a customer waiting
        if (args->salon->waiting == 0 && args->salon->num_customers == 0) {
            return NULL;
        }
        else if (args->salon->waiting == 0) {
            // No customers, barber goes back to sleep
            printf("Barber %d goes to sleep\n", args->id);
            barber_free[args->id] = true;
            semaphore_wait(args->salon->barber_semaphores[args->id]); // wait for customer
            continue;
        }
        else
        {
            args->salon->waiting--;
            barber_free[args->id] = false;

            printf("Barber %d starts cutting hair\n", args->id);
            semaphore_signal(args->salon->customer_semaphores[args->salon->waiting]);
            sleep(rand() % MAX_CUTTING_TIME + 2); // Simulate hair cutting
            printf("Barber %d finishes cutting hair\n", args->id);
            if (args->salon->waiting > 0) semaphore_signal(args->salon->customer_semaphores[args->salon->waiting--]);
            sleep(1);

            
            barber_free[args->id] = true;
        }
    }
}

void* customer_thread(void* arg) {
    CustomerThreadArgs *args = (CustomerThreadArgs *) arg;
    semaphore_wait(args->salon->customer_semaphores[args->salon->waiting]);

    if (args->salon->waiting > args->salon->num_chairs) {
        // No more available seats, customer leaves
        printf("Customer id:%d leaves without haircut\n", args->id);
        args->salon->num_customers--;
        return NULL;
    }
    else
    {
        // Take a seat in the waiting room
        args->salon->waiting++;

        // Wait for a barber
        int barber_id = seek_for_barber(args);
        if (barber_id == -1)
        {
            printf("Customer id %d in queue\n", args->id);
            args->salon->waiting++;
            semaphore_wait(args->salon->customer_semaphores[args->salon->waiting]);
        }
        else if (args->salon->waiting < args->salon->num_customers )
        {
            printf("Customer id:%d hi, I want a haircut\n", args->id);
            sleep(1);
            semaphore_signal(args->salon->barber_semaphores[barber_id]);
            semaphore_wait(args->salon->customer_semaphores[args->salon->waiting]);
            printf("Customer id:%d I got a haircut\n", args->id);
            sleep(1);

            args->salon->num_customers--;
        }
    }
    return NULL;
}



int main(int argc, char *argv[]) 
{
    if (argc != 4) 
    {
        printf("Usage: %s M N P\n", argv[0]);
        return 1;
    }

    srand((unsigned) time(NULL));


    int num_barbers = atoi(argv[1]); // M
    int num_chairs = atoi(argv[2]); // N
    int queue_size = atoi(argv[3]); // P

    if (num_barbers <= 0 || num_chairs <= 0 || queue_size <= 0 || num_barbers < num_chairs) 
    {
        printf("Invalid input\n");
        return 1;
    }

    /* Initialize shared memory and semaphores */
    char *filename = getenv("HOME");
    Salon *salon = (Salon *) attach_memory_block(filename, sizeof(Salon));
    if (salon == NULL) {
        printf("Error initializing shared memory\n");
        exit(1);
    }

    int semaphore_key = ftok(filename, 1);
    if (semaphore_key == -1) {
        printf("Error creating semaphore key\n");
        exit(1);
    }

    // int num_clients = rand() % MAX_CLIENTS + 1;
    int num_clients = 15;
    printf("Number of customers: %d\n", num_clients);

    /* Initialize salon data */
    for (int i = 0; i < num_barbers; i++) salon->barber_semaphores[i] = create_semaphore(semaphore_key, 0);
    for (int i = 0; i < num_clients; i++) salon->customer_semaphores[i] = create_semaphore(semaphore_key, 1);
    salon->num_barbers = num_barbers;
    salon->num_chairs = num_chairs;
    salon->queue_size = queue_size;
    salon->num_customers = 0;
    salon->waiting = 0;
    salon->num_clients = num_clients;

    /* Start customers thread */
    pthread_t customer_threads[num_clients];
    for (int i = 0; i < num_clients; i++) {
        CustomerThreadArgs *args = (CustomerThreadArgs *) malloc(sizeof(CustomerThreadArgs));
        args->salon = salon;
        args->id = i;
        // salon->waiting++;
        salon->num_customers++;
        pthread_create(&customer_threads[i], NULL, customer_thread, (void *) args);
    }

    /* Start barbers threads */
    pthread_t barber_threads[num_barbers];
    for (int i = 0; i < num_barbers; i++) {
        BarberThreadArgs *args = (BarberThreadArgs *) malloc(sizeof(BarberThreadArgs));
        args->salon = salon;
        args->id = i;
        barber_free[args->id] = true;
        pthread_create(&barber_threads[i], NULL, barber_thread, (void *) args);
    }



    /* Starts simulation*/
    for (int i = 0; i < num_barbers; i++) pthread_join(barber_threads[i], NULL);
    for (int i = 0; i < num_clients; i++) pthread_join(customer_threads[i], NULL);

    /* Detach and destroy shared memory and semaphores */

    for (int i = 0; i < num_barbers; i++) delete_semaphore(salon->barber_semaphores[i]);
    for (int i = 0; i < num_clients; i++) delete_semaphore(salon->customer_semaphores[i]);
    
    detach_memory_block((char *) salon);
    destroy_memory_block(filename);

    return 0;
}
