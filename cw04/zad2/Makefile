CC = gcc
CFLAgs = -Wall
PROGRAM = first second third

all: $(PROGRAM)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
	@echo "Now type in ./main & and take PID"

test:
	./first
	@sleep 3
	./second
	@sleep 3
	./third

clean:
	rm -f *.o $(PROGRAM)