#include <iostream>
#include <thread>
#include <atomic>


void threadCallableFunction(int id, std::atomic<int> &amount)
{
    std::cout<<"Inside Thread : "<<id<<std::endl;
    for(int i = 0 ; i < 1000000; i++)
    amount.fetch_add(1);

    std::cout<<"Thread Ending : "<<id<<std::endl;
}

int main()
{
    std::cout<<"Inside Main Thread"<<std::endl;
    std::atomic<int> amount(0);

    std::thread t1(threadCallableFunction, 1, std::ref(amount));
    std::thread t2(threadCallableFunction, 2, std::ref(amount));

    if(t1.joinable())
    t1.join();

    if(t2.joinable())
    t2.join();

    std::cout<<"Amount : \n\tActual : "<<amount<<"\n\tExpected : "<<2000000<<std::endl;
    std::cout<<"Main Thread Ending"<<std::endl;
    return 0;
}