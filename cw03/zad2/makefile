CC = gcc
CFLAGS = -Wall
PROGRAMS = main
CATALOGUES = empty_dir . many_files_dir

all:  dirs $(PROGRAMS)

dirs:
	mkdir empty_dir
	touch file1 file2 file3
	mkdir many_files_dir
	cd many_files_dir
	touch {1..1000}.txt


%:%.c
	$(CC) $(CFLAGS) $< -o $@
	./$@ empty_dir
	./$@ .
	./$@ many_files_dir


clean:
	rm -rf *.o $(PROGRAMS) empty_dir many_files_dir file* *.txt

