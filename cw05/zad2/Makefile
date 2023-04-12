all:
	gcc -Wall main.c -o main

test:
	./main 0.000001 25000 
	./main 0.00001 25000 
	./main 0.00001 2500
	./main 0.0001 2500
	./main 0.0001 250 
	./main 0.00005 20000
	./main 0.0005 2000 
	./main 0.005 200 

clean:
	rm -rf ./main raport.txt