PROGRAM = IHelloWorld parent

all: $(PROGRAM)

%:%.c
	gcc -Wall $< -o $@
	./$@

clean:
	rm -rf parent IHelloWorld