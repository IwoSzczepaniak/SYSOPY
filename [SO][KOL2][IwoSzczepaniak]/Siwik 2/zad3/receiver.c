#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


#define KEY  "./queuekey"


struct message {
			long type;
			char text[20];
		} msg;

int main() {
        sleep(1);
        int val = 0;


	/**********************************
	Otworz kolejke systemu V "reprezentowana" przez KEY
	**********************************/
	key_t key = ftok(KEY, 0);
	int msqid = msgget(key, IPC_EXCL);



	while(1)
 	{	
	    /**********************************
	    Odczytaj zapisane w kolejce wartosci i przypisz je do zmiennej val
	    obowiazuja funkcje systemu V
	    ************************************/
	   	msgrcv(msqid, (void *) &msg, sizeof(msg), 0, 0);
		val = atoi(msg.text);
		printf("%d square is: %d\n", val, val*val);
 
	}

	/*******************************
	posprzataj
	********************************/
	msgctl(msqid, IPC_RMID, NULL);


     return 0;
   }
