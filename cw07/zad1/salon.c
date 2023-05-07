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

#include "common.h"
#include "shared_mem.h"

bool barber_free[MAX_BARBERS];
int seek_for_barber(CustomerThreadArgs *args);

void* barber_thread(void* arg);
void* customer_thread(void* arg);


int main(int argc, char *argv[]) 
{
    if (argc != 4) 
    {
        printf("Usage: %s M N P\n", argv[0]);
        return 1;
    }

    int num_barbers = atoi(argv[1]); // M
    int num_chairs = atoi(argv[2]); // N
    int num_waiting = atoi(argv[3]); // P

    if (num_barbers <= 0 || num_chairs <= 0 || num_waiting <= 0 || num_barbers < num_chairs) 
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

    /* Initialize salon data */
    for (int i = 0; i < num_barbers; i++) salon->barber_semaphores[i] = create_semaphore(semaphore_key, 1);
    salon->num_barbers = num_barbers;
    salon->num_chairs = num_chairs;
    salon->num_waiting = num_waiting;
    salon->num_customers = 0;
    salon->waiting = 0;
    salon->barber_sleep_time = 2;  /* Default value */

    /* Start barbers threads */
    pthread_t barber_threads[num_barbers];
    for (int i = 0; i < num_barbers; i++) {
        BarberThreadArgs *args = (BarberThreadArgs *) malloc(sizeof(BarberThreadArgs));
        args->salon = salon;
        args->id = i;
        barber_free[args->id] = true;
        pthread_create(&barber_threads[i], NULL, barber_thread, (void *) args);
    }

    /* Start customers thread */
    int num_clients = rand() % MAX_CLIENTS + 1;
    pthread_t customer_threads[num_clients];
    for (int i = 0; i < num_clients; i++) {
        CustomerThreadArgs *args = (CustomerThreadArgs *) malloc(sizeof(CustomerThreadArgs));
        args->salon = salon;
        args->id = i;
        pthread_create(&customer_threads[i], NULL, customer_thread, (void *) args);
    }

    /* Starts simulation*/
    for (int i = 0; i < num_barbers; i++) pthread_join(barber_threads[i], NULL);
    for (int i = 0; i < rand() % MAX_CLIENTS + 1; i++) pthread_join(customer_threads[i], NULL);

    /* Detach and destroy shared memory and semaphores */
    for (int i = 0; i < num_barbers; i++) delete_semaphore(salon->barber_semaphores[i]);
 
    detach_memory_block((char *) salon);
    destroy_memory_block(filename);

    return 0;
}


int seek_for_barber(CustomerThreadArgs *args)
{
    for (int i = 0; i < args->salon->num_barbers; i++)
    {
        if (barber_free[i] == true) return i;
    }
    return 0;
}

void* barber_thread(void* arg) {
    BarberThreadArgs *args = (BarberThreadArgs *) arg;
    while (1) {
        semaphore_wait(args->salon->barber_semaphores[args->id]); // wait for customer

        // Check if there is a customer waiting
        if (args->salon->waiting == 0) {
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
            sleep(rand() % MAX_CUTTING_TIME + 2); // Simulate hair cutting

            printf("Barber %d finishes cutting hair\n", args->id);
            barber_free[args->id] = true;
        }
    }
}

void* customer_thread(void* arg) {
    CustomerThreadArgs *args = (CustomerThreadArgs *) arg;

    if (args->salon->waiting >= args->salon->num_chairs) {
        // No more available seats, customer leaves
        printf("Customer id:%d leaves without haircut\n", args->id);
        return NULL;
    }

    // Take a seat in the waiting room
    args->salon->waiting++;

    // Wait for a barber
    int barber_id = seek_for_barber(args);

    printf("Customer id:%d starts getting a haircut\n", args->id);
    semaphore_signal(args->salon->barber_semaphores[barber_id]);
    printf("Customer id:%d finishes getting a haircut\n", args->id);
    return NULL;
}
