#ifndef COMMON_LIB_H
#define COMMON_LIB_H

#define BARBERS_AMOUNT 2  // M 
#define CHAIRS_AMOUNT 4  // N
#define QUEUE_SIZE 10 // P
#define HAIR_STYLES 3 // F
#define BLOCK_SIZE 4096
#define FILENAME "main.c"

typedef struct {
    int klientID; 
    int fryzjerID;
    int status; // 0 - free, 1 - occupied
} chair_t;

typedef struct {
    int free_chairs_amount;
    int client_amount;
    int queue[QUEUE_SIZE];
    chair_t chairs[CHAIRS_AMOUNT];
} salon_t;

#endif