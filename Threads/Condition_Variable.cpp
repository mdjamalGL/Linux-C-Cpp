/**
 * Mutex 10
 * 01. std::mutex : This is used for securing critical section and avoid race conditions.
 * 
 * 02. std::condition_variable : This gives the ability to block a thread that is waiting for an event to occur.
 *     When that event occurs, the other thread invokes the cvar.notify_one and cvar.notify_all to notify the thread that is
 *     waiting for the event.
 *     It works only with unique_lock().
 * 
 * 03. cvar.notify_one() : informs the thread blocked on the condition variable to check the predicate.
 * 04. cvar.notify_all() : informs all the threads blocked on the condition variable, might be useful in case of some 
 *     global event. 
 * 
 */
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <unistd.h>

long long int amount = 0;
std::mutex mtx1;
std::condition_variable cv;

void threadCallableWithdraw(int money)
{
    std::cout<<"Inside Thread Withdraw Function : "<<std::this_thread::get_id()<<std::endl;
    
    std::unique_lock<std::mutex> lock(mtx1);
    cv.wait(lock, [](){ return amount != 0 ? true : false;});

    std::cout<<"lock acquired by : "<<std::this_thread::get_id()<<std::endl;

    if(amount < money)
    {
        std::cout<<"Balance is low : "<<amount<<std::endl;
    }
    else
    {
        amount -= money;
        std::cout<<"Debited : "<<money<<std::endl;
        std::cout<<"New Balance : "<<amount<<std::endl;
    }

    std::cout<<"lock released by : "<<std::this_thread::get_id()<<std::endl;
}

void threadCallableDeposit(int money)
{
    std::cout<<"Inside Thread Deposit Function : "<<std::this_thread::get_id()<<std::endl;

    {
        std::lock_guard lock(mtx1);
        std::cout<<"lock acquired by : "<<std::this_thread::get_id()<<std::endl;

        amount += money;
        std::cout<<"New Balance : "<<amount<<std::endl;
        // This notify_one will inform the thread that is waiting from the cv. to again check for the predicate

        std::cout<<"lock released by : "<<std::this_thread::get_id()<<std::endl;
    }
    cv.notify_one();

}

int main()
{
    std::cout<<"Inside Main Thread : "<<std::this_thread::get_id()<<std::endl;

    std::thread t1(threadCallableWithdraw, 500);
    std::thread t2(threadCallableDeposit, 1000);

    if(t1.joinable())
    t1.join();

    if(t2.joinable())
    t2.join();

    std::cout<<"Main Thread Complete"<<std::endl;
    return  0;
}