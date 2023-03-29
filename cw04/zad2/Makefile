CC = gcc
CFLAgs = -Wall
PROGRAM = first second third

all: $(PROGRAM)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@
	@echo "Now type in ./main & and take PID"

test:
	@echo "Use this ID to kill process"
	./main
	

clean:
	rm -f *.o $(PROGRAM)