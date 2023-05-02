#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

#include "Message.h"



class Child
{
    public:
        Child(pid_t pid)
        {
            this->id = pid;
        };
        ~Child();

        pid_t GetPid()
        {
            return this->id;
        };

    private:
        pid_t id;

};