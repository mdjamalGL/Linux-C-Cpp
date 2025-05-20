/**
 * Mutex 03
 * 01. std::timed_mutex : This is used for securing critical section and avoid race conditions.
 *     It provides a mechanism whereby if the lock is not acquired instead of blocking forever it will check
 *     for locks availability for n secs, after which it executes alternate code from critical section.
 *     Hence, it provides a way out if lock is not acquired.
 * 
 * 02. we use try_lock_if(time) and unlock(), with the mutex to restrict the access of critical section
 *     If we take sleep_sec_1 = 6 and sleep_sec_2 = 2, we won't get the correct value for amount, as the second thread waited for
 *     2 seconds while first thread took > 6 seconds to process.
 *     Had we taken sleep_sec_1 = 1, we get the correct answer as expected.
 */
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <unistd.h>

long long int amount = 0;
std::timed_mutex tmtx;

void threadCallableFunction1(int secs)
{
    std::cout<<"Inside Thread w Function : "<<std::this_thread::get_id()<<std::endl;

    tmtx.lock();
    std::cout<<"lock acquired by : "<<std::this_thread::get_id()<<std::endl;

    for(int i = 1 ; i <= 1000000 ; i++)
    {
        amount++;
    }
    std::this_thread::sleep_for(std::chrono::seconds(secs));

    std::cout<<"lock released by : "<<std::this_thread::get_id()<<std::endl;
    tmtx.unlock();
}

void threadCallableFunction2(int secs)
{
    std::cout<<"Inside Thread w Function : "<<std::this_thread::get_id()<<std::endl;

    if(tmtx.try_lock_for(std::chrono::seconds(secs)))
    {
        std::cout<<"lock acquired by : "<<std::this_thread::get_id()<<std::endl;
        for(int i = 1 ; i <= 1000000 ; i++)
        {
            amount++;
        }

        std::cout<<"lock released by : "<<std::this_thread::get_id()<<std::endl;        
        tmtx.unlock();
    }
    else
    {
        std::cout<<"Could not Acquire Lock"<<std::endl;
    }
}

int main()
{
    std::cout<<"Inside Main Thread : "<<std::this_thread::get_id()<<std::endl;

    int sleep_sec_1 = 1;
    int sleep_sec_2 = 2;

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