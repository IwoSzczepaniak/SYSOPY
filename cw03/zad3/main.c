#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <linux/limits.h>


void traverse_directory(const char* path, const char* char_chain)
{
    DIR* dir = opendir(path);
    if (!dir)
    {
        perror(path);
        exit(1);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)))
    {
        char entry_path[PATH_MAX];
        snprintf(entry_path, sizeof(entry_path), "%s/%s", path, entry->d_name);

        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        struct stat statbuf;
        if (lstat(entry_path, &statbuf) == -1)
        {
            perror(entry_path);
            exit(1);
        }

        if (S_ISDIR(statbuf.st_mode))
        {
            pid_t pid = fork();
            if (pid == -1)
            {
                perror("fork");
                exit(1);
            }
            else if (pid == 0)
            {
                traverse_directory(entry_path, char_chain);
                exit(0);
            }
        }
        
        else if (S_ISREG(statbuf.st_mode) && statbuf.st_size < PATH_MAX)
        {
            FILE* fp = fopen(entry_path, "r");
            if (!fp)
            {
                perror(entry_path);
                exit(1);
            }

            char buffer[PATH_MAX];
            if (fgets(buffer, sizeof(buffer), fp) != NULL && strncmp(buffer, char_chain, strlen(char_chain)) == 0)
            {
                printf("%s %d\n", entry_path, getpid());
            }

            fclose(fp);
        }
    }

    closedir(dir);
}


int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Usage: %s <directory> <char_chain>\n", argv[0]);
        exit(1);
    }

    traverse_directory(argv[1], argv[2]);
    return 0;
}
