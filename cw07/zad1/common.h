#ifndef COMMON_LIB_H
#define COMMON_LIB_H

#define MAX_CLIENTS 20
#define MAX_CUTTING_TIME 4
#define MAX_BARBERS 10

typedef struct{
    int barber_semaphores[MAX_BARBERS];
    int num_barbers;
    int num_chairs;
    int num_waiting;
    int num_customers;
    int waiting;
    int barber_sleep_time; 
} Salon;

typedef struct {
    Salon *salon;
    int id;
} BarberThreadArgs;

typedef struct {
    Salon *salon;
    int id;
} CustomerThreadArgs;
#endif