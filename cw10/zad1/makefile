all:
	gcc -Wall -o client client.c -lpthread
	gcc -Wall -o server server.c -lpthread
clean:
	rm -rf server client command_history.txt

test1:
	./server 12345 /tmp/socket_path local
test2:
	./client Sema local /tmp/socket_path
test3:
	./client Bob local /tmp/socket_path
test4:
	./client Alice local /tmp/socket_path