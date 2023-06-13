#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <mqueue.h>


#define KEY "./queuekey"

int main(int argc, char* argv[])
{
 
 if(argc !=2){
   printf("Not a suitable number of program parameters\n");
   return(1);
 }

 /******************************************************
 Utworz kolejke komunikatow POSIX "reprezentowana" przez KEY
 Wyslij do niej wartosc przekazana jako parametr wywolania programu 
 Obowiazuja funkcje POSIX
 ******************************************************/
  mqd_t mq = mq_open(KEY, O_CREAT|O_RDWR, 0666, 0);
  char buffer[20];
  sprintf(buffer, "%d", atoi(argv[1]));
  mq_send(mq, buffer, strlen(buffer), 0);


  return 0;
}



