#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <ftw.h>

static off_t total_size = 0;

int print_file(const char *path, const struct stat *statbuf, int typeflag)
{
    if (!S_ISDIR(statbuf->st_mode)) {
        printf("%ld %s\n", (long)statbuf->st_size, path);
        total_size += statbuf->st_size;
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s <directory>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if (ftw(argv[1], print_file, 16) == -1) {
        perror("ftw");
        exit(EXIT_FAILURE);
    }

    printf("%ld total\n", (long)total_size);
    exit(EXIT_SUCCESS);
    return 0;
}
