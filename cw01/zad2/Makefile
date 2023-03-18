CC= gcc
CFLAGS=-Wall -lrt
LDFLAGS=-L../zad1 -lwc_array

all: test_static test_shared test_dynamic

test_static: test.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -static

test_shared: test.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS) -fPIC -shared

test_dynamic: test_dyn.c
	$(CC) $(CFLAGS) -o $@ $< $(LDFLAGS)  -ldl

clean:
	rm -f test_static test_shared test_dynamic
