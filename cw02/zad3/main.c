#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>

int main()
{
    DIR *dir;
    struct dirent *entry;
    struct stat buf;
    long long total_size = 0;

    if ((dir = opendir(".")) == NULL)
    {
        perror("opendir failed");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG)
        {
            char *filename = entry->d_name;
            if (stat(filename, &buf) == -1)
            {
                perror("stat failed");
                continue;
            }
            if (!S_ISDIR(buf.st_mode))
            {
                long long size = buf.st_size;
                printf(" %lld %s\n", size, filename);
                total_size += size;
            }
        }
    }

    closedir(dir);

    printf("%lld total\n", total_size);

    return 0;
}
