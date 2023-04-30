#include <unistd.h>
#include <string>

class ChildProcess
{
    private:
        pid_t pid;
        
        int id;

    public:

        bool is_muted = false;

        pid_t get_pid()
        {
            return pid;
        }

        void set_pid(pid_t pid)
        {
            this->pid = pid;
        }

        int get_id()
        {
            return id;
        }
        
        void set_id(int id)
        {
            this->id = id;
        }

        std::string get_info()
        {
            std::string res = "process id: " + std::to_string(pid) + " process num:" + std::to_string(id) + " is muted: "
             + std::to_string(is_muted) + '\n';
            return res;
        }
};