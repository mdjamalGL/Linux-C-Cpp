#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main()
{
    std::cout<<"Original Single Process | process id : "<<getpid()<<std::endl<<std::endl;
    pid_t child_pid = fork();
    int open_fd = open("demo.txt", O_RDWR | O_CREAT);

    if(child_pid == -1)
    {
        std::cout<<"Error in creating new Process"<<std::endl;
    }
    else if(child_pid == 0)
    {
        /**
         * Since fork creates exact copy of the calling process
         * the file descriptor table of the child now also points
         * to the same open system table connections, as evident from the
         * behaviour of being able to read demo_txt.
         * 
         * Although the open_fd is global varible, had it been otherwise
         * we could not have read the file.
         */
        char buf[120];
        std::cout<<"Inside New Created Process | process id : "<<getpid()<<" | parent id : "<<getppid()<<std::endl;
        std::cout<<"Also can access demo file : "<<read(open_fd, buf, sizeof(char)*120)<<std::endl<<std::endl;
    }
    else
    {
        char buf[120];
        std::cout<<"Inside Parent | process id : "<<getpid()<<std::endl;
        std::cout<<"Process id of new process : "<<child_pid<<std::endl;
        std::cout<<"Also can access demo file : "<<read(open_fd, buf, sizeof(char)*120)<<std::endl<<std::endl;
    }
    return 0;
}