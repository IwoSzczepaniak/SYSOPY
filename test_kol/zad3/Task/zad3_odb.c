#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct pak{
 int i;
 char lit;
};

int main (int lpar, char *tab[]){
  int w1;
  struct pak ob1;
// 1) utworzyc potok nazwany 'potok1'
//
  char* myfifo = "potok1";
  mkfifo(myfifo, 0666);
  int fd = open(myfifo, O_RDONLY); 
  while (1){
// 2) wyswietlic obiekt otrzymany z potoku
//
//
    read(myfifo, ob1, sizeof(ob1));
    printf("otrzymano: %d %c\n",ob1.i,ob1.lit); fflush(stdout);
  }

  close(fd);
  unlink(myfifo);
  return 0;
}