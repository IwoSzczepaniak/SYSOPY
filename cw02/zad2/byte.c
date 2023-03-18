#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int main(int argc, char *argv[])
{
    off_t offset;
    ssize_t bytes_read;
    char c;
    clock_t start_time, end_time;
    double elapsed_time;

    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int src_fd = open(argv[1], O_RDONLY);
    if (src_fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int output_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (output_fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    off_t file_size = lseek(src_fd, 0, SEEK_END);
    if (file_size == -1)
    {
        perror("lseek");
        exit(EXIT_FAILURE);
    }

    

    start_time = clock();
    for (offset = file_size - 1; offset >= 0; offset--)
    {
        if (lseek(src_fd, offset, SEEK_SET) == -1)
        {
            perror("lseek");
            exit(EXIT_FAILURE);
        }
        bytes_read = read(src_fd, &c, 1);
        if (bytes_read == -1)
        {
            perror("read");
            exit(EXIT_FAILURE);
        }
        if (bytes_read == 0)
        {
            break; 
        }
        if (write(output_fd, &c, 1) == -1)
        {
            perror("write");
            exit(EXIT_FAILURE);
        }
    }
    end_time = clock();

    elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    // printf("Byte version time elapsed: %.4f seconds\n", elapsed_time);

    FILE *fp;
    if ((fp = fopen("byte_pomiar_zad_2.txt", "w")) == NULL)
    {
        perror("opening error");
        exit(1);
    }
    fprintf(fp, "Byte version time elapsed: %.4f seconds\n", elapsed_time);
    fclose(fp);

    close(src_fd);
    close(output_fd);
    return 0;
}
