#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

struct pak{
 int i;
 char lit;
} ob1;
/*program przy kazdym uzyciu klawiszy ctrl-c (SIGINT) ma wyslac obiekt o1 przez
potok nazwany 'potok1'*/
//
//

int fd;

void handler(int sig)
{
  write(fd, &ob1, sizeof(ob1));
}

int main (int lpar, char *tab[]){
//
//
  ob1.i=0;
  ob1.lit='a';

  char* myfifo = "./potok1";

  fd = open(myfifo, O_WRONLY);
  signal(SIGINT, handler);

  while(1) {

    printf("%d %c\n",ob1.i,ob1.lit); fflush(stdout);
    ob1.i++;
    ob1.lit=ob1.lit<'e'?ob1.lit+1:'a';
    sleep(1);

  }

  return 0;
}
