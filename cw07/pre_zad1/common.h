#ifndef COMMON_LIB_H
#define COMMON_LIB_H


#define BARBERS_AMOUNT 2  // M 
#define CHAIRS_AMOUNT 4  // N
#define QUEUE_SIZE 10 // P
#define BLOCK_SIZE 4096
#define FILENAME "main.c"

typedef struct {
    int klientID; // indeks klienta
    int fryzjerID; // indeks fryzjera
    int status; // 0 - wolne, 1 - zajęte
} fotel_t;

typedef struct {
    int ilosc_wolnych_foteli;
    int ilosc_klientow;
    int kolejka[QUEUE_SIZE];
    fotel_t fotele[CHAIRS_AMOUNT];
} salon_t;

#endif