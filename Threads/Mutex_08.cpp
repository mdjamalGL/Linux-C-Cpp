/**
 * Mutex 08
 * 01. std::shared_mutex : This is used for securing critical section and avoid race conditions.
 * 
 * 02. std::shared_lock : allows many reader threads to acquire the lock on shared mutex
 *     via this lock, many thread can read concurrently.
 * 
 * 03. std::unique_lock : if we use unique_lock on the shared mutex we won't be able to acuire any other
 *     lock be it unique or shared_lock.
 * 
 */
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <unistd.h>

long long int amount = 0;
std::shared_mutex smtx;

void threadCallableGet(int secs)
{
    std::cout<<"Inside Thread Get Function : "<<std::this_thread::get_id()<<std::endl;
    
    std::shared_lock<std::shared_mutex> lock(smtx);
    std::cout<<"lock acquired by : "<<std::this_thread::get_id()<<std::endl;
    std::cout<<"Amount Get : "<<amount<<std::endl;
    std::cout<<"lock released by : "<<std::this_thread::get_id()<<std::endl;
}

void threadCallableSet(int secs)
{
    std::cout<<"Inside Thread Set Function : "<<std::this_thread::get_id()<<std::endl;

    std::unique_lock<std::shared_mutex> lock(smtx);
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

    int sleep_sec_1 = 5;
    int sleep_sec_2 = 2;

    std::thread t1(threadCallableSet, sleep_sec_1);
    std::thread t2(threadCallableGet, sleep_sec_2);

    if(t1.joinable())
    t1.join();

    if(t2.joinable())
    t2.join();

    std::cout<<"Main Thread Complete"<<std::endl;
    return  0;
}