#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

double func(double x) 
{
    return 4.0 / (x*x + 1.0);
}

int main(int argc, char* argv[]) 
{
    if (argc != 5) {
        printf("Usage: %s <a> <b> <cell_size>\n", argv[0]);
        return 1;
    }

    int i = atoi(argv[1]);
    double prog_num = atof(argv[2]);
    double cell_size = atof(argv[3]);
    char* myfifo = argv[4];


    double a = i * (1.0 / prog_num);
    double b = (i + 1) * (1.0 / prog_num);

    int fd = open(myfifo, O_WRONLY);

    double result = 0.0;
    double x;
    for (x = a + cell_size / 2.0; x < b; x += cell_size)
    {
        result += func(x) * cell_size;
    }

    char str_res[128];
    sprintf(str_res, "%lf", result);

    write(fd, str_res, strlen(str_res)+1);

    close(fd);

    return 0;
}
