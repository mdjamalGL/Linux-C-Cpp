/**
 * Mutex 09
 * 01. std::mutex : This is used for securing critical section and avoid race conditions.
 * 
 * 02. std::scoped_lock : This simplifies the process of acquiring and managing locks of one or more mutexes,
 *     in a deadlock safe manner and using RAII principle.
 * 
 *     It uses an algorithm similar to std::lock() internally, which attempts to lock all mutexes, without deadlock.
 * 
 */
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <unistd.h>

long long int amount = 0;
std::mutex mtx1;
std::mutex mtx2;

void threadCallableGet(int secs)
{
    std::cout<<"Inside Thread Get Function : "<<std::this_thread::get_id()<<std::endl;
    
    std::scoped_lock lock(mtx1, mtx2);
    std::cout<<"lock acquired by : "<<std::this_thread::get_id()<<std::endl;
    for(int i = 1 ; i <= 1000000 ; i++)
    {
        amount++;
    }
    std::cout<<"lock released by : "<<std::this_thread::get_id()<<std::endl;
}

void threadCallableSet(int secs)
{
    std::cout<<"Inside Thread Set Function : "<<std::this_thread::get_id()<<std::endl;

    std::scoped_lock lock(mtx1, mtx2);
    std::cout<<"lock acquired by : "<<std::this_thread::get_id()<<std::endl;
    for(int i = 1 ; i <= 1000000 ; i++)
    {
        amount++;
    }
    std::cout<<"lock released by : "<<std::this_thread::get_id()<<std::endl;      
}

int main()
{
    std::cout<<"Inside Main Thread : "<<std::this_thread::get_id()<<std::endl;

    int sleep_sec_1 = 0;
    int sleep_sec_2 = 0;

    std::thread t1(threadCallableSet, sleep_sec_1);
    std::thread t2(threadCallableGet, sleep_sec_2);

    if(t1.joinable())
    t1.join();

    if(t2.joinable())
    t2.join();

    std::cout<<"Amount : \n\tActual : "<<amount<<"\n\tExpected : "<<2000000<<std::endl;
    std::cout<<"Main Thread Complete"<<std::endl;
    return  0;
}