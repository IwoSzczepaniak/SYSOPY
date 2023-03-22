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
	./main .. "#include"
	@echo "\n"
	./main ../.. "#include"

	@echo "\n"
	./main . "asdf"
	@echo "\n"
	./main .. "asdf"
	@echo "\n"
	./main ../.. "asdf"

	@echo "\n"
	./main . "zadanie"
	@echo "\n"
	./main .. "zadanie"
	@echo "\n"
	./main ../.. "zadanie"

clean:
	rm -rf *.o $(PROGRAMS)