#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int fd[2];
int d = 0;
int i = 0;

/*program tworzy dwa procesy potomne. Proces macierzysty co sekund� wysy�a
SIGUSR1 do procesu potomnego 1. Proces potomny 1 po otrzymaniu sygna�u przesy�a
kolejn� liczb� przez potok nienazwany do procesu potomnego 2, kt�ry wyswietla
te liczbe */

//
//

void handler(int sig)
{
  i++;
}

int main (int lpar, char *tab[]){
  pid_t pid1, pid2;
  //
  //
  pipe(fd);
  pid1 = fork();

  if (pid1<0){
    perror("fork");
    return 1;
  }
  else if (pid1==0){//proces 1
    close(fd[0]);
    signal(SIGUSR1, handler);
    while(1)
    {
      wait(NULL);
      if (d!=i)
      {
        d = i;
        write(fd[1], &d, sizeof(d));
      }
    }
    close(fd[1]);
    return 0;
  }

  else{
    pid2 = fork();

    if(pid2<0){
      perror("fork");
      return 1;
    }

    else if (pid2==0)
    {//proces 2
      close(fd[1]);
      int res;
      int prev_res = -1;
      while(1){
        read(fd[0], &res, sizeof(res));
        if (res != prev_res)
        {
          printf("Otrzymano %d\n", res);
          prev_res = res;
        }
      }
      close(fd[0]);
      return 0;
    }

    else {
    close(fd[0]);
    close(fd[1]);

    while(1) {
      //
      printf("wyslano SIGUSR1\n");
      sleep(1);
      kill(pid1, SIGUSR1);
      }
    }

  }

  
  
  

  return 0;
}
