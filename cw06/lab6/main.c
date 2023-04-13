#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

typedef struct msgbuf{
    long mtype;
    char mtext[16];
}message_buf;


int main(int argc, char* argv[])
{
    int msgid, i;
    key_t key;
    key = ftok("progfile", 65);


    msgid = msgget(key, IPC_CREAT|0600);
    if(msgid == -1)
    {
        printf("Queue not created");
    }

    struct msgbuf msgnoo = {1, "hello world"};
    struct msgbuf msgnot = {2, "wow"};
    if(msgsnd(msgid, &msgnoo, sizeof(msgnoo), IPC_NOWAIT) == -1)
    {
        perror("Msgnd");
        exit(1);
    }

    if(msgsnd(msgid, &msgnot, sizeof(msgnot), IPC_NOWAIT) == -1)
    {
        perror("msgsnd");
        exit(1);
    }

    struct msqid_ds reader;

    msgctl(msgid, IPC_STAT, &reader);
    printf("Comunicate number %d\n", reader.msg_qnum);
    printf("Bite number %d\n", reader.msg_qbytes);
    printf("Last pid %d\n", reader.msg_lspid);
    


    struct msgbuf rcvnoo;
    struct msgbuf rcvnot;

    if(msgrcv(msgid, (void *) &rcvnoo, sizeof(rcvnoo), 0,IPC_NOWAIT) == -1)
    {
        perror("msgrcv");
        exit(1);
    }
    printf("First message: %s, pid: %d\n", rcvnoo.mtext);

    if(msgrcv(msgid, (void *) &rcvnot, sizeof(rcvnot), 0,IPC_NOWAIT) == -1)
    {
        perror("msgrcv");
        exit(1);
    }
    printf("Second message: %s\n", rcvnot.mtext);

    msgctl(msgid, IPC_RMID, &reader);

    return 0;   
}

    