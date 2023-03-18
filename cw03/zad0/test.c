#include <stdio.h>
int main()
{
    for (int i = 0; i < 10; i++)
    {
        printf("Poczatek\n");
        int frk = fork();
        if (frk > 0)
        {
            int new = getpid();
            printf("Koniec current i:%d| fork:%d | getpid:%d\n", i, frk, new);
        }
        else
        {
            break; // można też forkować jedynie w roocie
        }
    }
    return 0;
}