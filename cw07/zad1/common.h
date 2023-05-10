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
