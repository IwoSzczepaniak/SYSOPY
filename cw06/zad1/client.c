#include<unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h> 
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include "common.h"

key_t queueKey;
int queueID, serverID, clientID;

int init();
void list();
void to_all(char* message);
void to_one(char* message, int receiver);
void stop_client();
void handle_server_message();

int main()
{
    queueKey = ftok(PATH , getpid());
    queueID = msgget(queueKey, IPC_CREAT | 0666);
    key_t serverKey = ftok(PATH, MY_SERVER_ID);
    serverID = msgget(serverKey, 0);
    clientID = init();

    size_t len = 0;
    ssize_t command_size;
    char* command = NULL;

    handle_server_message();
    signal(SIGINT, stop_client);


    while(1)
    {
        printf("Type in command > ");

        command_size = getline(&command, &len, stdin);
        if (command_size == -1)
        {
            perror("getline");
            exit(EXIT_FAILURE);
        }
        command[command_size-1] ='\0';

        if (strcmp(command, "") == 0) 
        {
            continue;
        }

        char *current = strtok(command," ");
       
 
        printf("%s\n" , current);

        if(strcmp(current , "LIST") == 0)
        {
            list();
        }

        else if(strcmp(current , "STOP") == 0)
        {
            stop_client();
        }

        else if(strcmp(current , "2ONE") == 0)
        {
            current = strtok(NULL, " ");
            int reciever = atoi(current);
            printf("%d\n" , reciever);
            current = strtok(NULL, " ");

            to_one(current , reciever);
        
        }

        else if (strcmp(current , "2ALL") == 0)
        {
            current = strtok(NULL, " ");
            to_all(current);
        }

        else if (strcmp(current , "READ") == 0)
        {
            handle_server_message();
        }

        else
        {
            printf("Command has not been recognized\n");
        }

    }
    return 0;
}

void handle_server_message()
{
    CommandBuff *mybuff = malloc(sizeof(CommandBuff));

    if(msgrcv(queueID, mybuff, sizeof(CommandBuff),0,IPC_NOWAIT) >= 0 )
    {
        if(mybuff->command == 4)
        {
            printf("STOP command has been recived , exiting\n");
            stop_client();
        }
        else
        {
            struct tm tmp_time = mybuff -> time_struct;
            printf("Client %d has sent a message at %02d:%02d:%02d \n", mybuff->client_ID,tmp_time.tm_hour,tmp_time.tm_min,tmp_time.tm_sec);
            printf("Message : %s\n" , mybuff->message);
        }
    }
    free(mybuff);
}

int init()
{
    CommandBuff *mybuff = malloc(sizeof(CommandBuff));

    time_t current_time = time(NULL);
    mybuff->time_struct = *localtime(&current_time);
    mybuff->command = 5;
    mybuff->queue_Key = queueKey;
    msgsnd(serverID, mybuff, sizeof(CommandBuff),0);

    msgrcv(queueID, mybuff, sizeof(CommandBuff),0,0);
    int current_id = mybuff->client_ID;

    printf("My client ID is %d\n" , current_id);

    free(mybuff);
    return current_id;
}



void list()
{
    time_t current_time = time(NULL);

    CommandBuff *mybuff = malloc(sizeof(CommandBuff));

    mybuff->time_struct = *localtime(&current_time);
    mybuff->client_ID = clientID;
    mybuff->command = 1;

    msgsnd(serverID, mybuff, sizeof(CommandBuff),0);
    msgrcv(queueID, mybuff, sizeof(CommandBuff),0,0);

    free(mybuff);
}

void to_all(char* message)
{
    time_t current_time = time(NULL);

    CommandBuff *mybuff = malloc(sizeof(CommandBuff));

    mybuff->time_struct = *localtime(&current_time);
    mybuff->client_ID = clientID;
    mybuff->command = 2;
    strcpy(mybuff->message, message);
    msgsnd(serverID, mybuff, sizeof(CommandBuff),0);

    free(mybuff);
}

void to_one(char* message, int receiver)
{
    time_t current_time = time(NULL);

    CommandBuff *mybuff = malloc(sizeof(CommandBuff));

    mybuff->time_struct = *localtime(&current_time);
    mybuff->client_ID = clientID;
    mybuff->receiver_ID = receiver;
    mybuff->command = 3;
    strcpy(mybuff->message, message);
    msgsnd(serverID, mybuff, sizeof(CommandBuff),0);

    free(mybuff);
}


void stop_client()
{
    CommandBuff *mybuff = malloc(sizeof(CommandBuff));

    time_t current_time = time(NULL);
    mybuff->time_struct = *localtime(&current_time);
    mybuff->client_ID = clientID;
    mybuff->command = 4;

    msgsnd(serverID, mybuff, sizeof(CommandBuff),0);
    msgctl(queueID, IPC_RMID, NULL);

    free(mybuff);
}

