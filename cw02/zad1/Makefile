CC = gcc
CFLAGS = -Wall
PROGRAMS = version_lib version_system
LETTER_IN_TEXT = j
LETTER_TO_CHANGE = m
FILE = tester.txt

all: tr $(PROGRAMS)

tr:
	tr $(LETTER_IN_TEXT) $(LETTER_TO_CHANGE) < $(FILE) > tr_out.txt

%:%.c
	$(CC) $(CFLAGS) $< -o $@
	./$@  $(LETTER_IN_TEXT) $(LETTER_TO_CHANGE) $(FILE) $@_out.txt
	rm -rf $@
	@echo ""
	diff -s $@_out.txt tr_out.txt
	@echo ""



clean:
	rm -rf *.o $(PROGRAMS)
	rm -rf *out.txt