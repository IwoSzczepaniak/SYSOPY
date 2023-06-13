#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define KEY "./queuekey"
struct message {
			long type;
			char text[20];
		} msg;


int main(int argc, char* argv[])
{
 
 if(argc !=2){
   printf("Not a suitable number of program parameters\n");
   return(1);
 }

  /******************************************************
   Utworz kolejke komunikatow systemu V "reprezentowana" przez KEY
  Wyslij do niej wartosc przekazana jako parametr wywolania programu 
  Obowiazuja funkcje systemu V
  ******************************************************/
  key_t key = ftok(KEY, 0);
  int msqid = msgget(key, IPC_CREAT);
  int val = atoi(argv[1]);
  msg.type = 1;
  sprintf(msg.text, "%d", val);
  msgsnd(msqid, (void *) &msg, sizeof(msg), 0);

  
  return 0;
}



