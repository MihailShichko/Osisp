#include <iostream>
#include <sys/types.h>
#include <unistd.h> 
#include <fstream>
#include <vector>
#include <string.h>

using std::cout;
using std::endl;

std::vector<std::string> variables;

void GetRequiredEnvVarsFromFile(const char* path)
{
    std::ifstream env_file;
   
    env_file.open(path);
    if(!env_file.is_open())
    {
        cout << "file with required vars can not be opened =(" << endl;
        exit(0);
    }

    while(!env_file.eof())
    {
        std::string env_var;
        env_file >> env_var;
        variables.push_back(env_var);
    }

    env_file.close();
}

void PlusMethod()
{
    cout << "Envaronmental variables: " << endl;
    for(int i = 0; i < variables.size(); i++)
    {
        cout << "\t" <<variables[i] <<"="<<getenv(variables[i].c_str()) << endl;
    }
}

void MultiplyMethod(char* envp[])
{
    cout << "Envaronmental variables: " << endl;
    for(int i = 0; i < variables.size(); i++)
    {
        int j = 0;
        char* env_var = envp[j];
        while(env_var != NULL)
        {   
            char* key = strtok(env_var, "=");
            char* value = strtok(NULL, "=");
            if(strcmp(key, variables[i].c_str()) == 0)
            {
                printf("\t%s=%s\n", key, value);
                break;
            }

            j++;
            env_var = envp[j];
        }
    }
}

void AmpersantMethod()
{
    cout << "Envaronmental variables: " << endl;
    for(int i = 0; i < variables.size(); i++)
    {
        int j = 0;
        char* env_var = environ[j];
        while(env_var != NULL)
        {   
            char* key = strtok(env_var, "=");
            char* value = strtok(NULL, "=");
            if(strcmp(key, variables[i].c_str()) == 0)
            {
                printf("\t%s=%s\n", key, value);
                break;
            }

            j++;
            env_var = environ[j];
        }
    }
}

int main(int argc, char* argv[], char* envp[])
{
    cout << "Hello from child process" << endl;
    cout << "My name is " << argv[0] << endl;
    cout << "My pid: " << getpid() << endl << "Parent pid: " << getppid() << endl;

    GetRequiredEnvVarsFromFile(argv[1]);
    if(strcmp(argv[2], "+") == 0)
    {
        PlusMethod();
    }
    else if(strcmp(argv[2], "*") == 0)
    {
        MultiplyMethod(envp);
    }
    else
    {
        AmpersantMethod();
    }
    
    exit(0);
}