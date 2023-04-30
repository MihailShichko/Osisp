#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>

const char* CHILD_PATH;

char* FindEnvVarValue(char* key, char* envp[])
{
    int i = 0;
    char* env_var = envp[i];
    while(env_var != NULL)
    {   
        char* current_key = strtok(env_var, "=");
        char* value = strtok(NULL, "=");
        if(strcmp(current_key, key) == 0)
        {
            return value;
        }

        i++;
        env_var = envp[i];
    }

    std::cout << "there is no such env var as " << key << std::endl;
    return NULL;

}

static int sortComparer(const void* first, const void* second){
    return strcmp(*(const char**) first, *(const char**) second);
}

void sortEnvp(char* envp[])
{
    char** e = envp;
    while(*e) e++;
    qsort(envp, e - envp, sizeof(char*), sortComparer);
}

char* GenerateProcName(int counter)
{
    return strcat(strdup("Child_"), (char*)&counter);
}

void CallChild(char* args[], char* envp[])
{
    execle(
        CHILD_PATH,
        args[0],
        args[1],
        args[2],
        NULL,
        envp);
}

void Run(char* envp[])
{
    
    char command;
    int status = 0;
    int counter = 0;
    char* args[3];
    while(true)
    {
        std::cout << "Your Command: "; 
        std::cin >> command;
        switch (command)
        {
            case '+':
            {
                CHILD_PATH=getenv("CHILD_PATH");
                pid_t child_process = fork();
                if(child_process == -1)
                {
                    perror("fork");                    
                }
                else if(child_process == 0) 
                {
                    char* args[] {GenerateProcName(counter), "/home/mihail/C++/Osisp/LAB2/ChildProcess/Vars.txt", "+"};
                    CallChild(args, envp);
                }
                else 
                {
                    waitpid(child_process, &status, 0);
                    counter++;
                    continue;
                }
            }
            break;
             
            case '*':
            {
                CHILD_PATH = FindEnvVarValue(strdup("CHILD_PATH"), envp);
                pid_t child_process = fork();
                if(child_process == -1)
                {
                    perror("fork");                    
                }
                else if(child_process == 0)
                {
                    char* args[] {GenerateProcName(counter), "/home/mihail/C++/Osisp/LAB2/ChildProcess/Vars.txt", "*"};
                    CallChild(args, envp);
                }
                else
                {
                    waitpid(child_process, &status, 0);
                    counter++;
                    continue;
                }
            }
            break;         
            case '&':
            {
                CHILD_PATH = FindEnvVarValue(strdup("CHILD_PATH"), environ);
                pid_t child_process = fork();
                if(child_process == -1)
                {
                    perror("fork");                    
                }
                else if(child_process == 0)
                {
                    char* args[] {GenerateProcName(counter), "/home/mihail/C++/Osisp/LAB2/ChildProcess/Vars.txt", "&"};
                    CallChild(args, envp);
                }
                else
                {
                    waitpid(child_process, &status, 0);
                    counter++;
                    continue;
                }
                exit(0);
            break;
            }
            case 'q':
                std::cout << "GOODBUY BRO" << std::endl;
                exit(0);
            break;
            default:
                std::cout << "WRONG COMMAND" << std::endl;
                counter++;
                continue;
            break;
        }


    }
}

void ShowEnvVars(char* envp[])
{
    char* env_var;
    int i = 0;
    while((env_var = envp[i]) != NULL)
    {
        std::cout << env_var << std::endl;
        i++;
    }
}

int main(int argc, char* argv[], char* envp[])
{
    if(setenv("CHILD_PATH", "/home/mihail/C++/Osisp/LAB2/ChildProcess/Child", 1) == -1) 
    {
        perror("setenv");
    }

    sortEnvp(envp);
    ShowEnvVars(envp);
    Run(envp);
}