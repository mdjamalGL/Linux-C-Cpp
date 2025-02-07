#include <iostream>
#include <cstdlib>
#include <unistd.h>

int main()
{
    std::cout<<"Running ps -a from /bin/sh"<<std::endl<<std::endl;
    if(system(NULL))
    {
        int pr_ret = system("ps -a");
    }
    std::cout<<"sleep for 20 sec"<<std::endl;
    sleep(20);
    std::cout<<"This should be blocked. until the command completes"<<std::endl<<std::endl;
    return 0;
}