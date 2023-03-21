CC = gcc
CFLAGS = -Wall
PROGRAMS = main
DIR = many_files_dir/

all:  dirs $(PROGRAMS)

dirs:
	mkdir empty_dir
	mkdir many_files_dir
	touch $(DIR)file1 $(DIR)file2 $(DIR)file3
	touch $(DIR){1..1000}.txt


%:%.c
	$(CC) $(CFLAGS) $< -o $@
	@echo "\nThis is empty directory:"
	./$@ empty_dir
	@echo "\nThis is current directory:"
	./$@ .
	@echo "\nThis is directory with many files:"
	./$@ many_files_dir

clean:
	rm -rf *.o $(PROGRAMS) empty_dir many_files_dir

