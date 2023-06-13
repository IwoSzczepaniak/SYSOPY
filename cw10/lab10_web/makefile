PROGS = client server

all: $(PROGS)

%:%.c
	gcc $< -o $@

clean:	
	rm -rf ./$(PROGS)