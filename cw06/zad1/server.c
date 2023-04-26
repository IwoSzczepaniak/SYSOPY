#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <fcntl.h>
#include "common.h"


int server_queue, new_client_ID;
key_t client_queue[MAX_CLIENTS];

void stop_server();
void init(CommandBuff *mybuff);
void list(CommandBuff *mybuff);
void to_all(CommandBuff *mybuff);
void to_one(CommandBuff *mybuff);
void stop_client(CommandBuff *mybuff);
void save_command(CommandBuff *mybuff);

int main() 
{   
    printf("Server is starting\n");

    for (int i = 0 ; i < MAX_CLIENTS; i++)
    {
        client_queue[i]=-1;
    }

    key_t queueKey = ftok(PATH, MY_SERVER_ID);
    CommandBuff *mybuff = malloc(sizeof(CommandBuff));
    server_queue = msgget(queueKey, IPC_CREAT | 0666);
    if (server_queue == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("Server started, queue ID: %d\n", server_queue);

    signal(SIGINT, stop_server);

    while (1) {

        msgrcv(server_queue, mybuff, sizeof(CommandBuff), -6, 0);

    
        if (mybuff->command == 5) 
        {
            printf("Init command has been recieved\n");
            init(mybuff);
        
        }
        else if (mybuff->command == 1)
        {
            printf("List command has been recieved\n");
            list(mybuff);
        
        }
        else if (mybuff->command == 2)
        {
            printf("To all command has been recieved\n");
            to_all(mybuff);
        
        }
        else if (mybuff->command == 3)
        {
            printf("To one command has been recieved\n");
            to_one(mybuff);
        
        }
        else if (mybuff->command == 4) 
        {
            printf("Stop(client) command has been recieved\n");
            stop_client(mybuff);
        
        }
        else 
        {
            // If the message type is not recognized, send an error message back to the sender
            // break;
        }
        
        continue;

    }
    return 0;
}

void stop_server()
{
    CommandBuff *mybuff = malloc(sizeof(CommandBuff));

    for (int i = 0 ; i < MAX_CLIENTS; i++)
    {
        if (client_queue[i] != -1)
        {
            mybuff->command = 4;
            int client_queueID = msgget(client_queue[i],0);
            msgsnd(client_queueID, mybuff,sizeof(CommandBuff),0);
            msgrcv(server_queue, mybuff,sizeof(CommandBuff),5,0);
        }

    }

    msgctl(server_queue, IPC_RMID, NULL);
    exit(0);
}

void save_command(CommandBuff *mybuff)
{
    struct tm tmp_time = mybuff->time_struct;
    FILE *file = fopen("command_history.txt", "a");

    switch (mybuff->command)
    {
        case 5:
        {
            if(mybuff->client_ID == -1)
            {
                printf("Client limit has been reached\n");
            }
            else
            {
                printf("Server : New client has been initialized - Client ID: %d\n" ,mybuff->client_ID );
                fprintf(file, "Server : New client has been initialized - Client ID: %d\n", mybuff->client_ID);
            }
            break;
        }
        case 1:
        {
            fprintf(file, "Server : LIST command - Client ID: %d\n", mybuff->client_ID);
            break;
        }
        case 2:
        {
            fprintf(file, "Server : 2ALL command - Client ID: %d\n", mybuff->client_ID);
            fprintf(file, "Message: %s\n", mybuff->message);
            break;
        }
        case 3:
        {
            fprintf(file, "Server : 2ONE command - Client ID: %d receiver ID: %d\n", mybuff->client_ID , mybuff->receiver_ID);
            fprintf(file, "Message: %s\n", mybuff->message);
            break;
        }
        case 4:
        {
            fprintf(file, "Server : STOP command - Client ID: %d\n", mybuff->client_ID);
            break;
        }
    }
    fprintf(file, "sent at: %02d:%02d:%02d\n\n",tmp_time.tm_hour,tmp_time.tm_min,tmp_time.tm_sec);

    fclose(file);

}

void init(CommandBuff *mybuff)
{
    new_client_ID++;
    if (new_client_ID < MAX_CLIENTS - 1)
    {
        mybuff->client_ID = new_client_ID;
        client_queue[new_client_ID] = mybuff->queue_Key;
    }

    int client_queueID = msgget(mybuff->queue_Key,0600);
    msgsnd(client_queueID, mybuff, sizeof(CommandBuff),0);

    save_command(mybuff);
}


void list(CommandBuff *mybuff)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if(client_queue[i]!=-1)
        {
            printf("Client id: %d\n", i);
        }
    }
    int client_queueID = msgget(mybuff->queue_Key,0600);
    msgsnd(client_queueID, mybuff, sizeof (CommandBuff), 0);
    
    save_command(mybuff);
}

void to_all(CommandBuff *mybuff)
{
    for(int i =0 ; i < MAX_CLIENTS ; i++)
    {
        if(client_queue[i] != -1 && mybuff->client_ID != i)
        {
            int receiverqueueID = msgget(client_queue[i],0);
            msgsnd(receiverqueueID,mybuff,sizeof(CommandBuff), 0);
        }
    }
    save_command(mybuff);
}

void to_one(CommandBuff *mybuff)
{
    int receiverqueueID = msgget(client_queue[mybuff->receiver_ID], 0);
    msgsnd(receiverqueueID,mybuff,sizeof(CommandBuff), 0);
    save_command(mybuff);
}

void stop_client(CommandBuff *mybuff)
{
    client_queue[mybuff->client_ID] = -1;

    if(mybuff->client_ID < new_client_ID)
    {
        new_client_ID = mybuff->client_ID;
    }
    save_command(mybuff);
}