#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main()
{
    std::cout<<"Original Single Process | process id : "<<getpid()<<std::endl<<std::endl;
    pid_t child_pid = vfork();
    // int p_demo_var = 122;

    if(child_pid == -1)
    {
        std::cout<<"Error in creating new Process"<<std::endl;
    }
    else if(child_pid == 0)
    {
        std::cout<<"Inside child process"<<std::endl;
        char *file = "ls";
        char *argument [] = {"ls", "-la", NULL};
        // p_demo_var = 144;
        execlp(file, "ls", "-la", NULL);
        /**
         * With the intention is to use exec later, so new space is not defineed for the process
         * if the child process tries to update the memory values of parent error will occur
         * 
         * Error : stack smashing detected
         */
    }
    else
    {
        char buf[120];
        std::cout<<"Inside Parent | process id : "<<getpid()<<std::endl;
        std::cout<<"Process id of new process : "<<child_pid<<std::endl;
    }
    return 0;
}