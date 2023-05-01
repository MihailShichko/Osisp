#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#include "Message.h"

int queue;

#define MAX 3000

Message produce()
{
    Message message;
    message.size = rand() % 257;
    message.data = rand() % 23;
    return message;
}

void queue_write()
{

    msgsnd(queue, );
}

key_t open_queue()
{
    key_t key = ftok(".", 65);
    if(queue == -1)
    {
        perror("ftok");
    }
    
    queue = msgget(key, 0777 | IPC_CREAT);
    return queue;
}


int main()
{
    printf("PRODUCER CREATED\n");
    queue = open_queue();
    while(true)
    {
       
       
    }
    
    exit(0);
}