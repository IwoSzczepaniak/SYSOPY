CC = gcc
CFLAGS = -Wall -pedantic
PROGRAMS = main


all:  $(PROGRAMS)

%:%.c
	$(CC) $(CFLAGS) $< -o $@

test:
	@echo "\n"
	./main . "#include"
	@sleep 1
	@echo "\n"
	./main .. "#include"
	@sleep 1
	@echo "\n"
	./main ../.. "#include"
	@sleep 1

	@echo "\n"
	./main . "asdf"
	@sleep 1
	@echo "\n"
	./main .. "asdf"
	
	@echo "\n"
	./main ../.. "asdf"
	@sleep 1

	@echo "\n"
	./main . "zadanie"
	@sleep 1
	@echo "\n"
	./main .. "zadanie"
	@sleep 1
	@echo "\n"
	./main ../.. "zadanie"

test2:
	@echo "\n"
	./main ~ "#include"
	@sleep 1
	@echo "\n*Permission denied should occur here*:\n"
	./main /root "#include"


clean:
	rm -rf *.o $(PROGRAMS)