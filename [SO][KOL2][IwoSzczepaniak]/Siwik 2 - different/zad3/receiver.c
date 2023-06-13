#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <mqueue.h>


#define KEY  "./queuekey"


int main() {
        sleep(1);
        int val = 0;


	/**********************************
	Otworz kolejke POSIX "reprezentowana" przez KEY
	**********************************/
	mqd_t mq = mq_open(KEY, O_CREAT|O_RDWR, 0666, 0);

	


	while(1)
 	{	
	    /**********************************
	    Odczytaj zapisane w kolejce wartosci i przypisz je do zmiennej val
	    obowiazuja funkcje POSIX
	    ************************************/
		char buffer[20];
	   	mq_receive(mq, buffer, 20, 0);
		val = atoi(buffer);
		printf("%d square is: %d\n", val, val*val);
 	
	}

	/*******************************
	posprzataj
	********************************/
	mq_close(mq);
	mq_unlink(KEY);

     return 0;
   }
