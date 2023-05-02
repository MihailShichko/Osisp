#include <stdio.h>
#include <iostream>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <vector>
#include <signal.h>
#include <semaphore.h>

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#include "Child.h"
#include "Producer.h"
#include "Consumer.h"

std::vector<Child> producers;
std::vector<Child> consumers;
key_t queue;

sem_t items;
sem_t free_space;
sem_t mutex;

key_t create_messageQueue()
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

void kill_children(std::vector<Child> children)
{
    for(int i = 0; i < children.size(); i++)
    {
        kill(children[i].GetPid(), SIGKILL);
    }

    children.clear();
}

void createProducer()
{
    pid_t pid = fork();
    Producer producer(pid);
    if(pid == -1)
    {
        perror("fork");
    }
    else if(pid == 0)
    {
        
    }
    else
    {
        producers.push_back(producer);
    }

}

void ProducerProcess()
{
    sem_wait(&free_space);
    sem_wait(&mutex);
    //put
    sem_post(&mutex);
    sem_post(&items);
}

void ConsumerProcess()
{
    sem_wait(&items);
    sem_wait(&mutex);
    //get
    sem_post(&mutex);
    sem_post(&free_space);
    //consume
}

void createConsumer()
{
    pid_t pid = fork();
    Consumer consumer(pid);
    if(pid == -1)
    {
        perror("fork");
    }
    else if(pid == 0)
    {
        
    }
    else
    {
        consumers.push_back(consumer);
    }

}

void setupSemphores()
{
    sem_init(&free_space, 1, 5);
    sem_init(&items, 1, 0);
    sem_init(&mutex, 1, 1);
    
}

void destroySemaphores()
{
    sem_destroy(&free_space);
    sem_destroy(&mutex);
    sem_destroy(&items);
}

void engine()
{
    while(true)
    {
        show_menu();
        char action;
        std::cin >> action;

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
                kill_children(producers);
                kill_children(consumers);
                printf("children terminated");
                destroySemaphores();
                exit(1);
                break;
            }
            default:
                printf("Wrong option... once again:\n");
                show_menu();
                break;
        }

        usleep(100);
    }
}

int main()
{
    queue = create_messageQueue();
    engine();

}