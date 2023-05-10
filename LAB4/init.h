#pragma once

#include <sys/types.h>
#include <semaphore.h>
#include <signal.h>
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

extern sem_t items;
extern sem_t free_space;
extern sem_t mutex;

extern std::vector<pid_t> producers;
extern std::vector<pid_t> consumers;

extern key_t queue;

struct message
{
    u_char type;
    ushort hash;
    u_char size;
    u_char data[256];
};

struct msg_buf
{
    long mtype;
    message mtext;
};

void init();
void setupSemphores();
void destroySemaphores();
void atexitHandler();
void killChildren(std::vector<pid_t> children);
ushort createHash(const message* message);
