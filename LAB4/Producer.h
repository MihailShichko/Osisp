#pragma once
#ifndef PRODUCER_H
#define PRODUCER_H
#include "ChildAYE.h"

class Producer: public ChildAYE
{
    public:
        Producer(pid_t pid, key_t queue, sem_t& free_space, sem_t& items, sem_t& mutex) : ChildAYE(pid, queue, free_space, items, mutex){};
        ~Producer();

        MessageAYE produce()
        {
            MessageAYE message;
            message.data = rand();
            message.mod = message.data % 256;
            return message;
        };

        virtual void Process() override
        {
            MessageAYE message = this->produce(); 
            int msgQue;
            while(true)
            {
                sem_wait(&free_space);
                sem_wait(&mutex);
                //put
                printf("message produced");
                msgQue = msgget(queue, 0666 | IPC_CREAT);
                msgsnd(msgQue, &message, sizeof(MessageAYE), 0);
                sem_post(&mutex);
                sem_post(&items);
            }
        }
};
#endif