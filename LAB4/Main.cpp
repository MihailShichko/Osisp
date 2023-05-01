#include <stdio.h>
#include <iostream>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <vector>
#include <signal.h>

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

std::vector<pid_t> producers;
std::vector<pid_t> consumers;
key_t queue;


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

void kill_children(std::vector<pid_t> children)
{
    for(int i = 0; i < children.size(); i++)
    {
        kill(children[i], SIGKILL);
    }

    children.clear();
}

pid_t create_childProc(const std::string& path)
{
    pid_t child = fork();
    if(child == -1)
    {
        perror("fork");
    }
    else if(child == 0)
    {
        execl(path.c_str(), NULL, NULL);
    }
    
    return child;
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
            case 'p':
                producers.push_back(create_childProc("/home/mihail/C++/Osisp/LAB4/Producer"));
                break;
            case 'c':
                producers.push_back(create_childProc("/home/mihail/C++/Osisp/LAB4/Consumer"));
                break;
            case 'q':
                kill_children(producers);
                kill_children(consumers);
                printf("children terminated");
                exit(1);
                break;
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