#include "Child.h"

class Producer: public Child
{
    public:
        Producer(pid_t pid) : Child(pid){};
        ~Producer();

        Message produce()
        {
            Message message;
            message.data = rand();
            message.mod = message.data % 256;
        };
};