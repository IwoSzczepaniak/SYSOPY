#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Nazwa programu: %s \n", argv[0]);
    fflush(stdout);

    setbuf(stdout, NULL); // turn off bufforing

    execl("/bin/ls", "ls", argv[1], NULL);

    perror("execl");
    exit(EXIT_FAILURE);
    return 0;
}
