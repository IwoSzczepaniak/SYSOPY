CC = gcc
CFLAGS = -Wall -lrt

all: libwc_array.a libwc_array.so

libwc_array.a: libwc_array.o
	ar rcs $@ $^

libwc_array.so: libwc_array.o
	$(CC) -shared -o $@ $^ 

%.o: %.c wc_array.h
	$(CC) $(CFLAGS) -c $@ 

clean:
	rm -f $(OBJ) libwc_array.a libwc_array.so libwc_array.o

