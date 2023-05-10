#include <stdio.h>
#include <iostream>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <vector>
#include <signal.h>
#include <semaphore.h>
#include <stdio_ext.h>

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/wait.h>

#include "init.h"
#include "consumer.h"
#include "producer.h"

std::vector<pid_t> producers;
std::vector<pid_t> consumers;
key_t queue;

sem_t items;
sem_t free_space;
sem_t mutex;

int producer_counter = 0;
int consumer_counter = 0;

key_t createMessageQueue()
{
    key_t queue = ftok(".", 65);
    if(queue == -1)
    {
        perror("ftok");
    }

    return queue;
}

void show_menu()
{
    printf("p - create producer\nc - create consumer\nq - exit\n");
}

void engine()
{
    char action;
    show_menu();
    while(true)
    {
        __fpurge(stdin);
        action = getchar();
        switch (action)
        {
            case 'p':{
                createProducer();
                break;
            }
            case 'c':{
                createConsumer();
                break;
            }
            case 'q':{
                printf("Exit...");
                exit(EXIT_SUCCESS);
            }
            default:
                printf("Wrong option... once again:\n");
                break;
        }

    }
}



int main()
{
    createMessageQueue();
    init();
    engine();
}