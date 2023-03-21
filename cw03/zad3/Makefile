CC = gcc
CFLAGS = -Wall -pedantic
PROGRAMS = main


all:  $(PROGRAMS)

%:%.c
	$(CC) $(CFLAGS) $< -o $@

test:
	@echo "\n"
	./main . "#include"
	@echo "\n"
	./main ../.. "zadanie"
	@echo "\n"
	./main ../../../.. "#include"


clean:
	rm -rf *.o $(PROGRAMS)