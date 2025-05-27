/**
 * Mutex 05
 * 01. std::mutex : This is used for securing critical section and avoid race conditions.
 * 
 * 02. std::lock_guard : This is a wrapper that follows RAII, and working as basic locking mechanism.
 *     when the lock_guard object is created, it acquires the lock on the mutex in its constructor. When
 *     the object goes out of scope, its destrucotr is called where lock is released
 * 
 *     This is better than manual locking and unlocking, lock_guard is more clear and maintainable.
 * 
 *     It is also related to exception safety, while using manual locking if after locking, an exception occurs
 *     the lock won't be released, which may lead to deadlock. But with lock_guard object, wether the program works
 *     smoothly or has an exception the destructor will always be called, regardless of how the object goes out of scope
 *     and hence lock will be released.
 * 
 * Problems:
 * while the mutex has been locked via lock_guard we cannot unlock it manually, we have to wait till the lock_guard object
 * becomes out of scope.
 * 
 * No Deferred Locking : when the lock_guard object is created, lock is acuired immediately, there is no way to defer this 
 * locking and lock the at a later time.
 * 
 * Cannot move ownership, there is no functionality for move semantics.
 * 
 * Thus, lock_guard_is useful for simple, scoped mutex locking.
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

    std::lock_guard<std::mutex> lock(mtx);
    std::cout<<"lock acquired by : "<<std::this_thread::get_id()<<std::endl;

    for(int i = 1 ; i <= 1000000 ; i++)
    {
        amount++;
    }
    std::this_thread::sleep_for(std::chrono::seconds(secs));
    std::cout<<"lock released by : "<<std::this_thread::get_id()<<std::endl;
}

void threadCallableFunction2(int secs)
{
    std::cout<<"Inside Thread w Function : "<<std::this_thread::get_id()<<std::endl;

    std::lock_guard<std::mutex> lock(mtx);
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

    int sleep_sec_1 = 2;
    int sleep_sec_2 = 3;

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