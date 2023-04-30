#include <iostream>
#include <sys/time.h> 
#include <sys/signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include "Signals.h"

#define TIME 10000;

struct data
{
    int64_t a, b;
};

struct statistics
{
    size_t _00, _01, _10, _11;
    pid_t child_id, parent_id;
};

statistics stati;
data FIRST = {0,0};
data SECOND = {1,1};
data res = {0,0};
int counter = 0;

void setup_timer(itimerval* timer)
{
    timer->it_value.tv_sec = 0;
    timer->it_value.tv_usec = TIME;
    timer->it_interval.tv_sec = 0;
    timer->it_interval.tv_usec = TIME;

     if(setitimer(ITIMER_REAL, timer, NULL)){
        perror("setitimer");
        exit(errno);
    }
}

void send_request()
{
    union sigval msg = {Sig_Request};

    if(sigqueue(getppid(), SIGUSR1, msg)){
        perror("sigqueue");
        exit(errno);
    }

}

void reset()
{
    counter = 0;

    stati._00 = 0;
    stati._01 = 0;
    stati._10 = 0;
    stati._11 = 0;
}

void show_info()
{
    printf("ppid = %d, pid = %d, 00 = %d, 01 = %d, 10 = %d, 11 = %d\n", getppid(), getpid(), stati._00, stati._01, stati._10, stati._11);
}

void send_info()
{
    
    stati.child_id = getpid();
    stati.parent_id = getppid();
    int fifo = open("Chanel", O_WRONLY);

    if(write(fifo, &stati, sizeof(statistics)) == -1)
    {
        perror("write");
        exit(0);
    }

    close(fifo);

}

void send_message_end()
{
    union sigval msg = {Sig_Message_End};

    if(sigqueue(getppid(), SIGUSR1, msg)){
        perror("sigqueue");
        exit(errno);
    }
}

void alarm_handler(int sig)
{
  switch(2 * (res.a == 0) + (res.b == 0))
    {
        case 0:
            stati._00++;
            break;
        case 1:
            stati._01++;
            break;
        case 2:
            stati._10++;
            break;
        case 3:
            stati._11++;
        
    }

    counter++;
}

void parent_signal_handler(int sig, siginfo_t* info, void* context)
{
    switch (info->si_int)
    {
    case Sig_Responce:
        printf("Child got responce from parent\n");
        send_info();
        send_message_end();
        break;
    case Killed_Child:
        exit(0);
        break;
    default:
        break;
    }

}

void set_handlers()
{
    struct sigaction sigsetup;
    sigsetup.sa_flags = SA_SIGINFO;
    sigsetup.sa_sigaction = parent_signal_handler;
    
    if(sigaction(SIGUSR2, &sigsetup, NULL) == -1)
    {
        perror("SIGUSR2");
        exit(errno);
    }

    if(signal(SIGALRM, &alarm_handler) == SIG_ERR)
    {
        perror("SIGALRM");
        exit(errno);
    }

}


int main()
{
    printf("Child process %d created\n", getpid());
    int couter = 0;
    itimerval timer;
    setup_timer(&timer);
    set_handlers();

    while(true)
    {
        res = FIRST;
        res = SECOND;

        if(counter == 500)
        {
            send_request();
            pause();
            reset();
        }   
    }

    exit(0);
}
