#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <sys/times.h>
#include <dlfcn.h>
#include "../zad1/libwc_array.h"

#define BILLION 1000000000L
#define _POSIX_C_SOURCE 200809L

int main()
{
    char command[100], *result;
    const char *filename;
    int size, index;
    wc_array_t *array = NULL;
    // clock_t start_t, finish_t;
    // double duration_t;
    double user_time, system_time;
    // dobule real_time;
    // struct timespec rtime_start;
    // struct timespec rtime_end;
    struct tms tms_start;
    struct tms tms_end;

    void *handle = dlopen("../zad1/libwc_array.so", RTLD_LAZY);
    if(!handle){/*error*/}    
    wc_array_t* (*wc_array_create)(size_t);
    wc_array_create = dlsym(handle, "wc_array_create");
    if(dlerror() != NULL){printf("Init error");}
    void (*wc_array_free)(wc_array_t*);
    wc_array_free = dlsym(handle, "wc_array_free");
    if(dlerror() != NULL){printf("Destroy error");}
    int (*wc_array_count)(wc_array_t*, const char* );
    wc_array_count = dlsym(handle, "wc_array_count");
    if(dlerror() != NULL){printf("Counting error");}
    char* (*wc_array_get)(wc_array_t* , size_t );
    wc_array_get = dlsym(handle, "wc_array_get");
    if(dlerror() != NULL){printf("Counting error");}
    int (*wc_array_remove)(wc_array_t*, size_t );
    wc_array_remove = dlsym(handle, "wc_array_remove");
    if(dlerror() != NULL){printf("Delete error");}

    
    while (1)
    {
        printf("Enter a command: \n");
        result = fgets(command, 100, stdin);
        command[strcspn(command, "\n")] = 0;

        // Start of time measuring
        // start_t = clock();
        // clock_gettime(CLOCK_REALTIME, &rtime_start);
        times(&tms_start);


        if (result == NULL)
        {
            printf("Error: issue with input");
        }

        if (strncmp(command, "init", 4) == 0)
        {
            sscanf(command, "%*s %d", &size);
            array = wc_array_create(size);
            if (array != NULL)
            {
                printf("Array initialized\n");
            }
            else
            {
                printf("Error: failed to create an array\n");
                break;
            }
        }
        else if (strncmp(command, "count", 5) == 0)
        {
            if (array != NULL)
            {
                sscanf(command, "%*s %ms", &filename);
                if (wc_array_count(array, filename) != 0)
                {
                    printf("Error: failed to count words in file %s\n", filename);
                }
            }
            else
            {
                printf("Error: array not initialized\n");
            }
        }
        else if (strncmp(command, "show", 4) == 0)
        {
            if (array != NULL)
            {
                sscanf(command, "%*s %d", &index);
                char *piece_array = wc_array_get(array, index);
                printf("This is the array:\n%s", piece_array);
            }
            else
            {
                printf("Error: array not initialized\n");
            }
        }

        else if (strncmp(command, "delete", 6) == 0)
        {
            if (array != NULL)
            {
                sscanf(command, "%*s %d", &index);;
                if (wc_array_remove(array, index) == -1)
                {
                    printf("Block out of range");
                }
                else
                {
                printf("Blocks at index %d deleted\n", index);
                }
            }
            else
            {
                printf("Error: array not initialized\n");
            }
        }
        else if (strncmp(command, "destroy", 7) == 0)
        {
            if (array != NULL)
            {
                wc_array_free(array);
                printf("Array destroyed\n");
            }
            else
            {
                printf("Error: no array to delete\n");
            }
        }
        else if (strncmp(command, "exit", 4) == 0)
        {
            break;
        }
        else
        {
            printf("Invalid command\n");
        }
                // end of time measuring
        // finish_t = clock();
        // duration_t = (double)(finish_t - start_t) / CLOCKS_PER_SEC;
        // clock_gettime(CLOCK_REALTIME, &rtime_end);
        times(&tms_end);
        // real_time = ((rtime_end.tv_sec - rtime_start.tv_sec) + (rtime_end.tv_nsec - rtime_start.tv_nsec))/(pow(10, 9));
        user_time = (tms_end.tms_utime - tms_start.tms_utime)/sysconf(_SC_CLK_TCK);
        system_time = (tms_end.tms_stime - tms_start.tms_stime)/sysconf(_SC_CLK_TCK);
        // printing the results
        // printf("CPU time(clock_t) is: %2.3f seconds\n", duration_t);
        printf("Real time is commented - some mysterious error, user time is: %.10f, system time is %.10f", user_time, system_time);
    }
    dlclose(handle);
    return 0;
}
