/**
 * Functions used : 
 * 1. pthread_create
 * 2. pthread_join
 * 3. pthread_exit
 */

#include <iostream>
#include <pthread.h>
#include <unistd.h>

void *function_thread(void *arg)
{
    std::cout<<"Inside Thread handler"<<std::endl;
    sleep(5);
    std::cout<<"Getting out from Thread Handler"<<std::endl;
    return NULL;
}
int main()
{
    pthread_t thread;
    int c_ret = pthread_create(&thread, NULL, function_thread, NULL);
    if(c_ret != 0)
    {
        std::cerr<<"Error in Creating Thread"<<std::endl;
    }
    pthread_join(thread, NULL);
    sleep(2);
    std::cout<<"Exiting"<<std::endl;
    pthread_exit(NULL);
    std::cout<<"Returned to main"<<std::endl;
    return 0;
}