#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Usage: %s <char1> <char2> <input_file> <output_file>\n", argv[0]);
        return 1;
    }

    char char1 = argv[1][0];
    char char2 = argv[2][0];
    int input_fd, output_fd, bytes_read;
    char buffer[BUFSIZ];

    if ((input_fd = open(argv[3], O_RDONLY)) == -1) {
        perror("Error: open input file");
        return 2;
    }

    if ((output_fd = open(argv[4], O_WRONLY | O_CREAT, 0666)) == -1) {
        perror("Error: open output file");
        return 3;
    }

    while ((bytes_read = read(input_fd, buffer, BUFSIZ)) > 0) {
        for (int i = 0; i < bytes_read; i++) {
            if (buffer[i] == char1) {
                buffer[i] = char2;
            }
        }

        if (write(output_fd, buffer, bytes_read) == -1) {
            perror("write output file");
            return 4;
        }
    }

    close(input_fd);
    close(output_fd);

    return 0;
}
