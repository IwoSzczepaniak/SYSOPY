all:
	gcc -Wall -o client client.c -lpthread
	gcc -Wall -o server server.c -lpthread
clean:
	rm -rf server client command_history.txt

test:
	./server 12345 /tmp/socket_path
	./client Alice network 127.0.0.1:12345
	./client Bob local /tmp/socket_path
	./client Charlie network 127.0.0.1:12345
