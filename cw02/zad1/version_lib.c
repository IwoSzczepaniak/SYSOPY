#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <char1> <char2> <input_file> <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char letter1 = argv[1][0];
    char letter2 = argv[2][0];
    char buffer[BUFFER_SIZE];

    FILE *input_file = fopen(argv[3], "rb");
    if (!input_file) {
        perror("Error: open input file");
        exit(EXIT_FAILURE);
    }

    FILE *output_file = fopen(argv[4], "wb");
    if (!output_file) {
        perror("Error: open output file");
        exit(EXIT_FAILURE);
    }

    size_t amount;
    while ((amount = fread(buffer, 1, BUFFER_SIZE, input_file)) > 0) {
        for (size_t i = 0; i < amount; i++) {
            if (buffer[i] == letter1) {
                buffer[i] = letter2;
            }
        }

        fwrite(buffer, 1, amount, output_file);
    }

    fclose(input_file);
    fclose(output_file);
    return EXIT_SUCCESS;
}
