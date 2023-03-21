#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <linux/limits.h>

#define MAX_DIRS 50
static char visited_dirs[MAX_DIRS][PATH_MAX];
static int visited_dirs_count = 0;



int is_directory(const char* path)
{
    struct stat statbuf;
    if (stat(path, &statbuf) == -1)
    {
        perror(path);
        exit(1);
    }

    return S_ISDIR(statbuf.st_mode);
}

int print_file(const char* path, const char* filename, const char* char_chain)
{
    struct stat statbuf;
    if (stat(path, &statbuf) == -1)
    {
        perror(path);
        exit(1);
    }

    if (S_ISREG(statbuf.st_mode) && statbuf.st_size < PATH_MAX)
    {
        FILE* fp = fopen(path, "r");
        if (!fp)
        {
            perror(path);
            exit(1);
        }

        char buffer[PATH_MAX];
        if (fgets(buffer, sizeof(buffer), fp) != NULL && strncmp(buffer, char_chain, strlen(char_chain)) == 0)
        {
            printf("%s %d\n", path, getpid());
        }

        fclose(fp);
    }

    return 0;
}



int is_visited_dir(const char *path) {
    for (int i = 0; i < visited_dirs_count; i++) {
        if (strcmp(path, visited_dirs[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void mark_visited_dir(const char *path) {
    if (visited_dirs_count < MAX_DIRS) {
        strcpy(visited_dirs[visited_dirs_count], path);
        visited_dirs_count++;
    } else {
        fprintf(stderr, "Too many visited directories, stopping search.\n");
        exit(1);
    }
}



void traverse_directory(const char* path, const char* char_chain)
{

    if (is_visited_dir(path) == 1) {
        return;
    }
    mark_visited_dir(path);

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

        if (is_directory(entry_path))
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
        else
        {
            print_file(entry_path, entry->d_name, char_chain);
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
