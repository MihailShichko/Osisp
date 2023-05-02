#include "Child.h"

class Consumer: public Child
{
    public:
        Consumer(pid_t pid) : Child(pid){};
        ~Consumer();

        void Consume(Message message)
        {
            printf("CONSUMED\n info: mod:%d data:%d\n", message.mod, message.data);
        };        
};