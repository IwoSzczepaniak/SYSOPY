#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h> 
#include <sys/ipc.h>
#include <sys/sem.h>

#include "common.h"
#include "shared_mem.h"

salon_t *salon;

struct sembuf semafor_salon;
struct sembuf semafor_chair[CHAIRS_AMOUNT];
struct sembuf semafor_barber[BARBERS_AMOUNT];
struct sembuf semafor_queue;

void barber(int id);
void client(int id);
void take_chair(int id_klienta);
void free_chair(int id_klienta);
void take_queue(int id_klienta);
void free_queue(int id_klienta);

int main()
{
    
}