#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>

int main()
{
    int pid = getpid();
    std::cout<<"My process Id : "<<pid<<std::endl;

    int oldfd = open("text.txt", O_WRONLY, 0644);
    write(oldfd, "written using write\n", 20);

    std::cout<<"old fd : "<<oldfd<<std::endl;
    std::cout<<"changing it using dup()"<<std::endl;
    oldfd = dup(oldfd);
    std::cout<<"new fd : "<<oldfd<<std::endl;
    std::cout<<"written using cout 1"<<std::endl;
    
    //dup2(oldfd ,newfd);
    dup2(oldfd, 1);
    std::cout<<"written using cout 2"<<std::endl;
    return 0;

}