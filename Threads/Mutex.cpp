/**
 * Mutex 01
 * 01. std::mutex : This is used for securing critical section and avoid race conditions.
 *     It provides exclusive, non recursive ownership.
 * 02. we use lock() and unlock(), with the mutex to restrict the access of critical section
 *     without mutex, the value of amount will always be less than 2000000.
 */
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <unistd.h>

long long int amount = 0;
std::mutex mtx;

void threadCallableFunction(int secs)
{
    std::cout<<"Inside Thread w Function : "<<std::this_thread::get_id()<<std::endl;

    mtx.lock();
    for(int i = 1 ; i <= 1000000 ; i++)
    {
        amount++;
    }
    mtx.unlock();
}

int main()
{
    std::cout<<"Inside Main Thread : "<<std::this_thread::get_id()<<std::endl;

    std::thread t1(threadCallableFunction, 1);
    std::thread t2(threadCallableFunction, 1);

    if(t1.joinable())
    t1.join();

    if(t2.joinable())
    t2.join();

    std::cout<<"Amount : \n\tActual : "<<amount<<"\n\tExpected : "<<2000000<<std::endl;
    std::cout<<"Main Thread Complete"<<std::endl;
    return  0;
}