/**
 * Mutex 06
 * 01. std::mutex : This is used for securing critical section and avoid race conditions.
 * 
 * 02. std::unique_lock : Another template for managing mutexes, that is more flexible than lock_guard
 *     and follow RAII.
 *     
 *     Default Locking : similar to locking at the time of creation of lock object.
 *
 *     Deferred Locking : Apart from locking the mutex at the time of creating unique_lock, we can defer
 *     the actual locking and perform it later manually, we can also release it manually, or it will do itself 
 *     at the end of the scope.
 * 
 */
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <unistd.h>

long long int amount = 0;
std::mutex mtx;

void threadCallableFunction1(int secs)
{
    std::cout<<"Inside Thread w Function : "<<std::this_thread::get_id()<<std::endl;

    std::unique_lock<std::mutex> lock(mtx, std::defer_lock);
    std::this_thread::sleep_for(std::chrono::seconds(secs));

    lock.lock();
    std::cout<<"lock Deferred acquired by : "<<std::this_thread::get_id()<<std::endl;

    for(int i = 1 ; i <= 1000000 ; i++)
    {
        amount++;
    }
    std::cout<<"lock released by : "<<std::this_thread::get_id()<<std::endl;
}

void threadCallableFunction2(int secs)
{
    std::cout<<"Inside Thread w Function : "<<std::this_thread::get_id()<<std::endl;

    std::unique_lock<std::mutex> lock(mtx);
    std::cout<<"lock acquired by : "<<std::this_thread::get_id()<<std::endl;
    for(int i = 1 ; i <= 1000000 ; i++)
    {
        amount++;
    }
    std::this_thread::sleep_for(std::chrono::seconds(secs));
    std::cout<<"lock released by : "<<std::this_thread::get_id()<<std::endl;        
}

int main()
{
    std::cout<<"Inside Main Thread : "<<std::this_thread::get_id()<<std::endl;

    int sleep_sec_1 = 5;
    int sleep_sec_2 = 1;

    std::thread t1(threadCallableFunction1, sleep_sec_1);
    std::thread t2(threadCallableFunction2, sleep_sec_2);

    if(t1.joinable())
    t1.join();

    if(t2.joinable())
    t2.join();

    std::cout<<"Amount : \n\tActual : "<<amount<<"\n\tExpected : "<<2000000<<std::endl;
    std::cout<<"Main Thread Complete"<<std::endl;
    return  0;
}