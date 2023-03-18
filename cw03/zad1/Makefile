CC = gcc
CFLAGS = -Wall
PROGRAMS = main
AMOUNT = 16

all:  $(PROGRAMS)

%:%.c
	$(CC) $(CFLAGS) $< -o $@
	./$@  $(AMOUNT)

clean:
	rm -rf *.o $(PROGRAMS)
