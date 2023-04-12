#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char* argv[])
{
    printf("hi");

    if (argc != 3)
    {
        printf("Insert: %s <cell_size> <prog_num>\n", argv[0]);
        return 1;
    }

    clock_t start = clock();
    int prog_num = atoi(argv[2]);
    char * myfifo = "/tmp/myfifo";

    mkfifo(myfifo, 0666);

    double result_sum = 0.0;

    for (int i = 0; i < prog_num; i++)
    {
        char i_str[32];
        snprintf(i_str, sizeof(i_str), "%d", i);

        char *args[] = {"./sub", i_str, argv[2], argv[1], myfifo, NULL};
        execvp(args[0], args);


        char arr1[64];
        int fd = open(myfifo, O_RDONLY);
        read(fd, arr1, sizeof(arr1));
        close(fd);

        double res;
        sscanf(arr1, "%lf", &res);
        result_sum += res;
    }

    unlink(myfifo);
    clock_t end = clock();
    FILE *raport = fopen("raport.txt", "a");
    fprintf(raport, "Cell size: %s, \tNumber of programs: %s", argv[1], argv[2]);
    fprintf(raport, ", Result: %f", result_sum);
    fprintf(raport, ", Time: %fs\n", (float)(end - start) / CLOCKS_PER_SEC);
    fclose(raport);

    return 0;
}
