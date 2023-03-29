CC = gcc
CFLAgs = -Wall
PROGRAM = catcher sender

all: $(PROGRAM)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
	
clean:
	rm -f *.o $(PROGRAM)