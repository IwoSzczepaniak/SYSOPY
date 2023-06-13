#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/sem.h>
#include <fcntl.h>
#include <sys/ipc.h>


#define FILE_NAME "common.txt"
#define SEM_NAME "my_kol_sem"


void print_sem_value(sem_t *sem){
      int sem_val;
      sem_getvalue(sem,&sem_val);
      printf("Current sem value: %d\n",sem_val);	
}

int main(int argc, char** args){

   if(argc !=4){
    printf("Not a suitable number of program parameters\n");
    return(1);
   }

      
   /************************************
   Utworz semafor systemu V. Ustaw jego wartosc na 1
   *************************************/
   key_t key = ftok(FILE_NAME, 0);
   int semid = semget(key, 0, IPC_CREAT|0666);
   semctl(semid, 0, SETVAL, 1);


   

   print_sem_value(sem_id);
        
     
   int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC , 0644);
   
   int parentLoopCounter = atoi(args[1]);
   int childLoopCounter = atoi(args[2]);
      
   char buf[20];
   pid_t childPid;
   int max_sleep_time = atoi(args[3]);
   
   if(childPid=fork()){
      int status =0;
      srand((unsigned)time(0)); 

      while(parentLoopCounter--){
         int s = rand()%max_sleep_time+1;
         sleep(s);    
            
      print_sem_value(sem_id);

         /******************************************
       Sekcja krytyczna. Zabezpiecz dostep semaforem
         ******************************************/
      struct sembuf sem_buf;
      sem_buf.sem_num = 0;
      sem_buf.sem_op = -1;
      sem_buf.sem_flg = 0;
      semop(semid, &sem_buf, 1);

      print_sem_value(sem_id);

         
         sprintf(buf, "Wpis rodzica. Petla %d. Spalem %d\n", parentLoopCounter,s);
      write(fd, buf, strlen(buf));
      write(1, buf, strlen(buf));
         
      print_sem_value(sem_id);

         /****************************************************
       Koniec sekcji krytycznej
         ****************************************************/
      sem_buf.sem_num = 0;
      sem_buf.sem_op = 1;
      sem_buf.sem_flg = 0;
      semop(semid, &sem_buf, 1);
        
         print_sem_value(sem_id);


        }
        waitpid(childPid,&status,0);
     }
     else{

	srand((unsigned)time(0)); 
      while(childLoopCounter--){

         int s = rand()%max_sleep_time+1;
         sleep(s);                

         print_sem_value(sem_id);
            
	    /******************************************
	    Sekcja krytyczna. Zabezpiecz dostep semaforem
	    ******************************************/
         struct sembuf sem_buf;
         sem_buf.sem_num = 0;
         sem_buf.sem_op = -1;
         sem_buf.sem_flg = 0;
         semop(semid, &sem_buf, 1);

         print_sem_value(sem_id);
               
         sprintf(buf, "Wpis dziecka. Petla %d. Spalem %d\n", childLoopCounter,s);
         write(fd, buf, strlen(buf));
         write(1, buf, strlen(buf));

         print_sem_value(sem_id);

	    /****************************************************
	    Koniec sekcji krytycznej
	    ****************************************************/
         sem_buf.sem_num = 0;
         sem_buf.sem_op = 1;
         sem_buf.sem_flg = 0;
         semop(semid, &sem_buf, 1);


         print_sem_value(sem_id);

        }
        _exit(0);
     }
     
    /***************************************
    Posprzataj semafor
    ***************************************/
   semctl(semid, 0, IPC_RMID, 0);
   close(fd);
   return 0;
}
     
        
