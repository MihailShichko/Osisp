#include "consumer.h"
 
 
 #include <sys/mman.h>

void consume(message message)
{
    ushort msg_hash = message.hash;
    message.hash = 0;
    ushort cur_hash = createHash(&message);

    if(msg_hash != cur_hash){
        fprintf(stderr, "error: corrupted message consumed: %d -> %d\n", msg_hash, cur_hash);
    }

    message.hash = msg_hash;
}

void createConsumer()
{
    pid_t child = fork();
    switch (child)
    {
    case -1:
        perror("fork");
        break;
    case 0:
        break;
    default:
        consumers.push_back(child);
        consumer_counter++;
        return;
    }

    printf("CONSUMER PROCESS:\n");
    while (true)
    {
        showMessage(getMessage());
        sleep(5);
    }
}

void showMessage(message message)
{
    printf("consumer %d\n"
            "message:\n"
            "\ttype: %d\n"
            "\thash: %hu\n"
            "\tsize: %d\n\n",
            getpid(), message.type, message.hash, message.size);
}

message getMessage()
{
    /*printf("getMessage\n");
    message message;
    int msgQue = msgget(queue, 0666 | IPC_CREAT);
    if(msgQue == -1)
    {
        printf("CANT PUT MESSAGE\n");
        perror("msgget");
    }
    sem_wait(&items);
    sem_wait(&mutex);
    msgrcv(msgQue, &message, sizeof(message), 1, 0);
    printf("recieved\n");
    sem_post(&mutex);
    sem_post(&free_space);
    return message;*/

    printf("getMessage\n");
    msg_buf message;
    int msgQue = msgget(queue, 0666 | IPC_CREAT);
    if(msgQue == -1)
    {
        printf("CANT PUT MESSAGE\n");
        perror("msgget");
    }

    sem_wait(&items);// ne propuskaet
    sem_wait(&mutex);
    printf("BEFORE RECIEVING\n");
    if(msgrcv(msgQue, &message, sizeof(message), 1, 0) == -1)
    {
        printf("CANT RECIEVE\n");
        perror("msgrcv");
    }

    printf("RECEIVED\n");
    sem_post(&mutex);
    sem_post(&free_space);
    return message.mtext;
}

