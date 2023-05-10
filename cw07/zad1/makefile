CC=gcc
CFLAGS=-Wall

all: library progs
	
library:
	$(CC) $(CFLAGS) -c shm_sem.c -o shm_sem.o
	ar rcs libshm_sem.a shm_sem.o

progs:
	$(CC) $(CFLAGS) salon.c -L. -lshm_sem -o salon

test:
	./salon 5 3 5

clean:
	rm -rf *.a *.o salon
