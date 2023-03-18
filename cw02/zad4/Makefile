CC = gcc
CFLAGS = -Wall
PROGRAMS = main
DIRECTORY = .

all: $(PROGRAMS)
	./main $(DIRECTORY)
	rm -rf main
	@echo "\ncompare to:\n"
	find $(DIRECTORY) -type f| xargs wc --bytes

%:%.c
	$(CC) $(CFLAGS) $< -o $@
	 
clean:
	rm -rf *.o $(PROGRAMS) 