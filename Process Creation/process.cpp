#include <iostream>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    int pid = getpid();
    std::cout<<"My process Id : "<<pid<<std::endl;

    int forkRes = fork();
    if(forkRes == 0)
    {
        std::cout<<"Parent : "<<getppid()<<" Child : "<<getpid()<<std::endl;
    }
    else if(forkRes > 0)
    {
        std::cout<<"Parent : "<<getpid()<<" Child : "<<forkRes<<std::endl;
    }
}