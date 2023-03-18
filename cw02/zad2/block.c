#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

#define BLOCK_SIZE 1024

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1)
    {
        perror("open source file failed");
        exit(EXIT_FAILURE);
    }

    int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (dest_fd == -1)
    {
        perror("open destination file failed");
        exit(EXIT_FAILURE);
    }

    off_t file_size = lseek(src_fd, 0, SEEK_END);
    if (file_size == -1)
    {
        perror("lseek failed");
        exit(EXIT_FAILURE);
    }

    char buffer[BLOCK_SIZE];
    clock_t start_time, end_time;
    double elapsed_time;

    start_time = clock();
    off_t offset = 0;
    off_t remaining_size = file_size;
    off_t block_size;
    while (offset < file_size)
    {
        remaining_size -= offset;
        block_size = remaining_size < BLOCK_SIZE ? remaining_size : BLOCK_SIZE;

        if (lseek(src_fd, -block_size, SEEK_CUR) == -1)
        {
            perror("lseek failed");
            exit(EXIT_FAILURE);
        }

        if (read(src_fd, buffer, block_size) != block_size)
        {
            perror("read failed");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < block_size / 2; i++)
        {
            char tmp = buffer[i];
            buffer[i] = buffer[block_size - i - 1];
            buffer[block_size - i - 1] = tmp;
        }

        if (write(dest_fd, buffer, block_size) != block_size)
        {
            perror("write failed");
            exit(EXIT_FAILURE);
        }

        offset += block_size;
    }
    end_time = clock();
    elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // printf("Block version time elapsed: %.4f seconds\n", elapsed_time);

    FILE *fp;
    if ((fp = fopen("block_pomiar_zad_2.txt", "w")) == NULL)
    {
        perror("opening error");
        exit(1);
    }
    fprintf(fp, "Block version time elapsed: %.4f seconds\n", elapsed_time);
    fclose(fp);

    close(src_fd);
    close(dest_fd);
    return 0;
}
