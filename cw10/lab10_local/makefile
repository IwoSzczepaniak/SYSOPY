PROGS = client server stop_client

all: $(PROGS)

%:%.c
	gcc $< -o $@

clean:	
	rm -rf ./$(PROGS)