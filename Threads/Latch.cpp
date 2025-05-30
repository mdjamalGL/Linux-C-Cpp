/**
 * Latch : A single use couting primitive, that makes a thread wait / blocked until N tasks are completed.
 * We define the Latch object with the max counter value.
 * 
 * Latch.count_down() : decrements the value of the counter.
 * Latch.wait() : waits for the internal counter to reach 0, before that all the threads block at this point, 
 *      when we reach the count to zero, every thread that is currently blocked or thread that will call wait,
 *      after this will resume its processing.
 * 
 * Its a one time gate, and thus cannot be reset.
 * 
 */

#include <iostream>
#include <thread>
#include <latch>

int amount = 0;
std::mutex mtx;
void threadCallableFunction(int id, std::latch &ltc)
{
    std::cout<<"Inside Thread : "<<id<<std::endl;

    {
        std::lock_guard<std::mutex> lguard(mtx);
        std::cout<<"\tLock Acquired by : "<<id<<std::endl;

        for(int i = 1 ; i <= 1000000 ; i++)
        {
            amount++;
        }
    }
    ltc.count_down();

    std::cout<<"Completed thread : "<<id<<std::endl;
}

int main()
{
    std::cout<<"Inside Main Thread "<<std::endl;
    std::latch ltc(2);

    std::thread t1(threadCallableFunction, 1, std::ref(ltc));
    std::thread t2(threadCallableFunction, 2, std::ref(ltc));

    ltc.wait();

    if(t1.joinable())
    t1.join();

    if(t2.joinable())
    t2.join();

    std::cout<<"Amount : \n\tActual : "<<amount<<"\n\t Expected : "<<2000000<<std::endl;
    std::cout<<"Main Thread Complete"<<std::endl;
}