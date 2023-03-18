CC = gcc
CFLAGS = -Wall
INPUT = ../zad1/tester.txt
PROGRAMS = byte block

all: tac $(PROGRAMS) res

tac:
	tac < $(INPUT) | rev > tmp_tac.txt

%:%.c
	$(CC) $(CFLAGS) $< -o $@
	./$@  $(INPUT) tmp_$@1.txt
	./$@  tmp_$@1.txt tmp_$@2.txt
	rm -rf $@
	@echo ""
	diff -s $(INPUT) tmp_$@2.txt
	@echo ""


res:
	cat byte_pomiar_zad_2.txt block_pomiar_zad_2.txt > pomiar_zad2.txt
	rm -rf byte_pomiar_zad_2.txt block_pomiar_zad_2.txt

clean:
	rm -rf *.o $(PROGRAMS) *.txt
