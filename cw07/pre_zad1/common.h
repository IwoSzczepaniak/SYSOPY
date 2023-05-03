#ifndef COMMON_LIB_H
#define COMMON_LIB_H


#define BARBERS_AMOUNT 2  // M 
#define CHAIRS_AMOUNT 4  // N
#define QUEUE_SIZE 10 // P
#define BLOCK_SIZE 4096
#define FILENAME "main.c"

#define SEM_PRODUC_KEY 1234
#define SEM_RECEIVER_KEY 5678

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