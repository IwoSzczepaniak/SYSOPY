CC = gcc
CFLAGS = -Wall -pedantic
PROGRAMS = main
START = ./main

all:  $(PROGRAMS)

%:%.c
	$(CC) $(CFLAGS) $< -o $@

test:
	@echo "\n"
	$(START) . "#include"
	@sleep 1

	@echo "\n"
	$(START) .. "#include"
	@sleep 1

	@echo "\n"
	$(START) ../.. "#include"
	@sleep 1

	@echo "\n"
	$(START) . "asdf"
	@sleep 1

	@echo "\n"
	$(START) .. "asdf"
	@sleep 1

	@echo "\n"
	$(START) ../.. "asdf"
	@sleep 1

	@echo "\n"
	./$(PROGRAMS) . "zadanie"
	@sleep 1

	@echo "\n"
	$(START) .. "zadanie"
	@sleep 1

	@echo "\n"
	$(START) ../.. "zadanie"

test2:
	@echo "\n"
	$(START) ~ "#include"
	@sleep 2

	@echo "\n*Permission denied* should occur here, for:"
	$(START) /root "#include"


clean:
	rm -rf *.o $(PROGRAMS)