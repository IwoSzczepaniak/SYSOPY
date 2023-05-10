#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main(int argc, char*argv[])
{
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if (pid == 0) 
    {
        close(fd[1]); 
        char buffer[100];
        read(fd[0], buffer, sizeof(buffer));
        printf("Received data: %s\n", buffer);
    }
    else
    {
        close(fd[0]);
        char *message = "Hello world";
        write(fd[1], message, strlen(message) + 1); 
    }

}