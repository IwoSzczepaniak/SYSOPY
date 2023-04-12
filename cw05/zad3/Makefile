CC = gcc -Wall
PROGRAMS = main sub

all: $(PROGRAMS)

%: %.c
	$(CC) $< -o $@

clean:
	rm -rf ./main ./sub