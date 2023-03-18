#include <stdio.h>
#include <sys/types.h>

int main()
{
    int child_pid;
    int value = 100;
    child_pid = vfork();
    
    printf("%d\n", value);
    if (child_pid != 0)
    {
        printf("Ten napis zostal wyswietlony w programie 'main'!\n");
    }
    else
    {
        execvp("./IHelloWorld", NULL);
        printf("jestem po execu - nie dzialam juz tutaj");
    }

    return 0;
}