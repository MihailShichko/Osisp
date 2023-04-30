#include <iostream>
#include <string>
#include <algorithm>
#include <signal.h>
#include <vector>
#include <stdio_ext.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include "ChildProcess.h"
#include "Signals.h"

// смотри в чем понт, судя по всему, когда дочерний процесс что-то вписывает в поток, ему нужно сделать exit(), чтобы вернуть поток вывода бате


using std::string;

int count = 0;
int Timer_Sec = 5;
std::vector<ChildProcess> Children;
sigset_t empty_mask;
sigset_t block_mask;

struct statistics
{
    size_t _00, _01, _10, _11;
    pid_t child_id, parent_id;
};

struct hard_command{
    char command;
    int index = -1;
};

int get_num_from_str(string& str)
{
    auto it = std::find_if(str.begin(), str.end(), isdigit);
    if(it != str.end()) return atoi(str.c_str() + (it - str.begin()));  
    return 0;
}

struct hard_command parse(string& command)
{
    struct hard_command res;
    res.command = command[0];
    res.index = get_num_from_str(command);
    return res;
}
    
ChildProcess find_process_Id(int num)
{
    for(int i = 0; i < Children.size(); i++)
    {
        if(Children[i].get_id() == num) return Children[i];
    }

    std::cout << "process cant be found" << std::endl;
}

ChildProcess find_process_Pid(pid_t pid)
{
    for(int i = 0; i < Children.size(); i++)
    {
        if(Children[i].get_pid() == pid) return Children[i];
    }

    std::cout << "process cant be found" << std::endl;
}

void show_process_list()
{
    for(int i = 0; i < Children.size(); i++)
    {
        std::cout << Children[i].get_info();
    }
}

void kill_child(pid_t id)
{
    kill(id, SIGTERM);
    Children.erase(Children.begin());
    std::cout << "Child process number " << count << " deleted!!!" << std::endl;
    count--;
}

void kill_children()
{
    while(count != 0)
    {
         kill_child(Children.begin()->get_pid());
    }
}

void mute_process(int id)
{
    Children[id].is_muted = true;
}

void mute_every_process()
{
    for(int i = 0; i < Children.size(); i++)
    {
        mute_process(i);
    }
}

void unmute_process(int id)
{
    Children[id].is_muted = false;
}

void unmute_every_process()
{
     for(int i = 0; i < Children.size(); i++)
    {
        unmute_process(i);
    }
}

void mute_except(ChildProcess except)
{
    for(int i = 0; i < Children.size(); i++)
    {
        if(Children[i].get_pid() == except.get_pid()) continue;
        mute_process(i);
    }
}

void timer_handler(int sig){
    unmute_every_process();
}

void send_message(pid_t pid, Parent_Signals responce)
{
    union sigval message = {responce};

    if(sigqueue(pid, SIGUSR2, message)){
        perror("sigqueue");
        exit(errno);
    }
}

void show_info(statistics info)
{
    printf("Child info: ppid = %d, pid = %d, 00 = %d, 01 = %d, 10 = %d, 11 = %d\n", info.parent_id, info.child_id, info._00, info._01, info._10, info._11);
}

void get_message()
{
    int fifo = open("Chanel", O_RDONLY);
    statistics result;
    if(read(fifo, &result, sizeof(statistics)) == -1)
    {
        perror("read");
        exit(0);
    } 

    show_info(result);
    close(fifo);
}

void child_handler(int sig, siginfo_t* info, void* context)
{
    pid_t pid = info->si_pid;
    ChildProcess temp = find_process_Pid(pid);

    switch (info->si_int)
    {
    case Sig_Request:
        if(!temp.is_muted)
        {
            printf("--------------------------------INFO SECTION--------------------------------\n");
            send_message(pid, Sig_Responce);
            get_message();
        }
        break;
    case Sig_Message_End:       
        printf("--------------------------------INFO SECTION END----------------------------\n");
        break;
    default:
        std::cout << "Unknown signal" << std::endl;
        break;
    }

    __fpurge(stdin);
}


void setup_handlers()
{
    if(signal(SIGALRM, &timer_handler) == SIG_ERR)
    {
        perror("SIGALRM");
        exit(errno);
    }

    struct sigaction sigsetup;
    sigsetup.sa_flags = SA_SIGINFO;
    sigsetup.sa_sigaction = child_handler;
    if(sigaction(SIGUSR1, &sigsetup, NULL))
    {
        perror("SIGUSR1");
        exit(errno);
    }
}


int exec_command(hard_command command)
{
    int res = 0;
    switch(command.command)
    {
        case '+':
        {     
            count++;
            ChildProcess child;
            child.set_id(count);
            pid_t pid = fork();
            if(pid == -1)
            { 
                perror("fork");
                exit(0);
            }
            if(pid == 0) 
            {
                execl("/home/mihail/C++/Osisp/LAB3/Child", NULL, NULL);   
            }
            else
            {
                child.set_pid(pid);
                Children.push_back(child);
            }    

            res = 1;
            break;
        }
        case '-':
            kill_child(Children.back().get_pid());
            res = 1;
            break;
        case 'l':
        {
            printf("Here is list: \n");
            show_process_list();
            res = 1;
            break;
        }
        case 'k':
        {
            kill_children();
            res = 1;
            break;
        }
        case 's':
        {
            if(command.index == -1)
            {
                mute_every_process();
                printf("All processes muted\n");
            }
            else
            {
                mute_process(command.index);
                printf("process muted\n");
            }
            res = 1;
            break;
        }
        case 'g':
        {
            if(command.index == -1)
            {
                unmute_every_process();
                printf("All processes unmuted\n");
            }
            else
            {
                unmute_process(command.index);
            }

            res = 1;
            break;
        }
        case 'p':            
            mute_except(find_process_Id(command.index));
            send_message(find_process_Id(command.index).get_pid(), Sig_Responce);
            alarm(Timer_Sec);
            res = 1;
            get_message();

            break;
        case 'q':
        {
            kill_children();
            std::cout << "All children were killed" << std::endl;
            res = -1;
            break;
        }
        default:
        {
            //std::cout <<"Wrong command" << std::endl;
            res =  1;
        }
    }

    return res;
}

hard_command get_command()
{
    struct hard_command aye;
    string comm;
    std::cin.clear();
    __fpurge(stdin);
    std::cin >> comm;
    if(comm.size() > 1)
    {
        aye = parse(comm);
    }
    else
    {
        aye.command = comm[0];
    }

    return aye;
}

int main()
{
    if(mkfifo("Chanel", 0777) == -1)
    {
        if(errno != EEXIST)
        {
            perror("mkfifo");
            exit(0);
        }
    }

    setup_handlers();
    while(true)
    {
        if(exec_command(get_command()) == -1) break;
    }
    exit(0);
}