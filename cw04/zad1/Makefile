CC = gcc
CFLAGs = -Wall 
PROGRAMS = main

all: $(PROGRAMS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

test:
	./main ignore fork
	@sleep 1
	@echo "\n"
	./main ignore exec
	@sleep 1
	@echo "\n"

	./main mask fork
	@sleep 1
	@echo "\n"
	./main mask exec
	@sleep 1
	@echo "\n"

	./main pending fork
	@sleep 1
	@echo "\n"
	./main pending exec
	@sleep 1
	@echo "\n"

	./main handler fork
	@sleep 1
	@echo "\n"


clean:
	rm -f $(PROGRAMS)