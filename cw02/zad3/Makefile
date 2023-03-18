CC = gcc
CFLAGS = -Wall
PROGRAMS = main

all: $(PROGRAMS) 		

%:%.c
	$(CC) $(CFLAGS) $< -o $@
	./$@
	rm -rf $@
	@echo "\ncompare to:\n"
	wc --bytes * 
	

clean:
	rm -rf *.o $(PROGRAMS) tmp*.txt 

