#pragma once
#ifndef CONSUMER_H
#define CONSUMER_H
#include "ChildAYE.h"

class Consumer: public ChildAYE
{
    public:
        Consumer(pid_t pid, key_t queue, sem_t& free_space, sem_t& items, sem_t& mutex) : ChildAYE(pid, queue, free_space, items, mutex){}; 
        ~Consumer();

        virtual void Process() override
        {
            MessageAYE message; 
            int msgQue;
            while(true)
            {
                sem_wait(&items);
                sem_wait(&mutex);
                //get
                msgrcv(msgQue, &message, sizeof(MessageAYE), 1, 0);
                printf("recieved");
                sem_post(&mutex);
                sem_post(&free_space);
                //consume
                this->Consume(message);
            }
        };

    private:
        void Consume(MessageAYE message)
        {
            printf("CONSUMED\n info: mod:%d data:%d\n", message.mod, message.data);
        };      
};

#endif