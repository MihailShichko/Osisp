#include "producer.h"


extern int producer_counter;

message produce()
{
    struct message msg;
    msg.size = rand() % 256;
    msg.type = rand() % 0x8;

    for(ushort i = 0; i < msg.size; i++){
        msg.data[i] = rand() % 256;
    }

    msg.hash = 0;
    //message.hash = createHash(&message);//тут проблема
    return msg;
}

void createProducer()
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
        producers.push_back(child);
        producer_counter++;
        return;
    }

    printf("PRODUCER PROCESS:\n");
    while (true)
    {
        msg_buf buf;
        buf.mtype = 1; 
        buf.mtext = produce();
        putIntoQueue(buf);
       /*putIntoQueue(produce());*/
        sleep(5);
    }
    

}

void putIntoQueue(msg_buf message)
{
    printf("putIntoQueue\n");
    sem_wait(&free_space);
    sem_wait(&mutex);
    printf("message produced\n");
    int msgQue = msgget(queue, 0666 | IPC_CREAT);
    if(msgQue == -1)
    {
        printf("CANT PUT MESSAGE\n");
        perror("msgget");
    }
    if(msgsnd(msgQue, &message, sizeof(message), 0) == -1)
    {
        printf("CANT SEND\n");
        perror("msgsnd");
    }

    printf("MESSAGE SENT\n");
    sem_post(&mutex);
    sem_post(&items);
}

/*void putIntoQueue(message message)
{
    printf("putIntoQueue\n");
    sem_wait(&free_space);
    sem_wait(&mutex);
    printf("message produced\n");
    int msgQue = msgget(queue, 0666 | IPC_CREAT);
    if(msgQue == -1)
    {
        printf("CANT PUT MESSAGE\n");
        perror("msgget");
    }
    msgsnd(msgQue, &message, sizeof(message), 0);
    sem_post(&mutex);
    sem_post(&items);
}*/
