#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

double func(double x)
{
    return 4/(x*x + 1);
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        printf("Insert: %s <cell_size> <proc_num>\n", argv[0]);
        return 1;
    }

    clock_t start = clock();

    double cell_size = atof(argv[1]);
    int proc_num = atoi(argv[2]);
    int fd[proc_num][2];


    for (int i = 0; i < proc_num; i++)
    {
        if (pipe(fd[i]) == -1)
        {
            perror("pipe");
            return 1;
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            return 1;
        }
        else if (pid == 0)
        {
            close(fd[i][0]);

            double a = i * (1.0 / proc_num);
            double b = (i + 1) * (1.0 / proc_num);

            double result = 0.0;
            double x;
            for (x = a + cell_size / 2.0; x < b; x += cell_size)
            {
                result += func(x) * cell_size;
            }


            write(fd[i][1], &result, sizeof(result));
            close(fd[i][1]);
            return 0;
        }
        else
        {
            close(fd[i][1]);
        }
    }

    double sum = 0.0;
    for (int i = 0; i < proc_num; i++)
    {
        double result;
        read(fd[i][0], &result, sizeof(result));
        sum += result;
        close(fd[i][0]);
        wait(NULL);
    }
    clock_t end = clock();

    FILE *raport =fopen("raport.txt", "a"); 
    fprintf(raport, "Cell size: %s, \tNumber of processes: %s", argv[1], argv[2]);
    fprintf(raport, ", \tResult: %f", sum);
    fprintf(raport, ", \tTime: %fs\n", (float)(end - start) / CLOCKS_PER_SEC);

    return 0;
}
