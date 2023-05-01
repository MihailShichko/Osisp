#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

int queue;

void consume(){}

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
    printf("CONSUMER CREATED\n");
    queue = open_queue();
    
    while(true)
    {
        //usleep(2000);
    }

    exit(0);
}