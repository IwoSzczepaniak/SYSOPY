CC=gcc
CFLAGS=-Wall

all: library progs
	
library:
	$(CC) $(CFLAGS) -c shared_mem.c -o shared_mem.o
	ar rcs libshared_mem.a shared_mem.o

progs:
	$(CC) $(CFLAGS) salon.c -L. -lshared_mem -o salon

test:
	./salon 5 3 5

clean:
	rm -rf *.a *.o salon
