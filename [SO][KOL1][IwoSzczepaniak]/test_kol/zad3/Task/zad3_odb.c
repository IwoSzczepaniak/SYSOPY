#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


struct pak{
 int i;
 char lit;
};

int main (int lpar, char *tab[]){
  int w1;
  struct pak ob1, ob2;
// 1) utworzyc potok nazwany 'potok1'
//
  char* myfifo = "potok1";
  mkfifo(myfifo, 0666);
  int fd = open(myfifo, O_RDONLY); 
  while (1){
// 2) wyswietlic obiekt otrzymany z potoku
//
//
    read(fd, &ob2, sizeof(ob2));
    if (&ob1 != &ob2)
    {
      printf("otrzymano: %d %c\n",ob1.i,ob1.lit); fflush(stdout);
      ob1 = ob2;
    }
  }

  close(fd);
  unlink(myfifo);
  return 0;
}