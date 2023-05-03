#pragma once
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#include "Message.h"


class ChildAYE
{
    public:
        ChildAYE(pid_t pid, key_t queue, sem_t& free_space, sem_t& items, sem_t& mutex);
        ~ChildAYE();

        pid_t GetPid();
        
        virtual void Process() = 0;
    private:
        pid_t id;
    protected:
        key_t queue;
        sem_t items;
        sem_t free_space;
        sem_t mutex;
};

ChildAYE::ChildAYE(pid_t pid, key_t queue, sem_t& free_space, sem_t& items, sem_t& mutex)
{
    this->id = pid;
    this->queue = queue; 
    this->items = items;
    this->free_space = free_space;
    this->mutex = mutex;
};

pid_t ChildAYE::GetPid()
{
    return this->id;
}; 