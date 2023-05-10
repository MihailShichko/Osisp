#include "init.h"

void init()
{
    setupSemphores();
    atexit(atexitHandler);

    printf("Iitialisation is complete\n");
}

void setupSemphores()
{
    if(sem_init(&free_space, 1, 5) || sem_init(&items, 1, 1) || sem_init(&mutex, 1, 1))
    {
        perror("sem_init");
    }
}

void destroySemaphores()
{
    sem_destroy(&free_space);
    sem_destroy(&mutex);
    sem_destroy(&items);
}

void atexitHandler()
{
    printf("Destroying semaphores...\n");
    destroySemaphores();
    printf("Killing consumers...\n");
    killChildren(consumers);
    printf("Killing producers...\n");
    killChildren(producers);
}

ushort createHash(const message* message)
{
    ushort hash = 5381;

    for(size_t i = 0; i < sizeof(message); i++){
        hash = ((hash << 5) + hash) + ((u_short*) message)[i];
    }

    return hash;
}

void killChildren(std::vector<pid_t> children)
{
    for(int i = 0; i < children.size(); i++)
    {
        kill(children[i], SIGKILL);
    }

    children.clear();
}
