CC = gcc
CFLAGs = -Wall 
PROGRAMS = main

all: $(PROGRAMS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(PROGRAMS)