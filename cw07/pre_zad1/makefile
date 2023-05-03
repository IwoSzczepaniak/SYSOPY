CC=gcc
CFLAGS=-Wall

all: library progs
	
library:
	$(CC) $(CFLAGS) -c shared_mem.c -o shared_mem.o
	ar rcs libshared_mem.a shared_mem.o

progs:
	$(CC) $(CFLAGS) main.c -L. -lshared_mem -o main
	$(CC) $(CFLAGS) read.c -L. -lshared_mem -o read
	$(CC) $(CFLAGS) destroy.c -L. -lshared_mem -o destroy

test:
	./main hello
	./read
	./destroy

clean:
	rm -rf *.a *.o read main destroy
