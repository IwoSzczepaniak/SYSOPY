CC = gcc
CFLAGS = -Wall -lrt
LDFLAGS=-L../zad1 -lwc_array
LIBDIR = ../zad1

all: test_static test_shared test_dynamic

test_static:
	cd $(LIBDIR) 
	$(CC) $(CFLAGS) -static ../zad2/test.c $(LDFLAGS) -o test_static
	./test_static < input.txt > results_static.txt
	rm -f test_static

test_shared: 
	cd $(LIBDIR) 
	$(CC) $(CFLAGS) ../zad2/test.c $(LDFLAGS) -Wl,-rpath,$(LIBDIR) -o test_shared
	./test_shared < input.txt > results_shared.txt
	rm -f test_shared

test_dynamic: ../zad2/test_dyn.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -ldl
	./test_dynamic < input.txt > results_dynamic.txt
	rm -f test_dynamic

clean:
	rm -f *.o results_*.txt test_*
