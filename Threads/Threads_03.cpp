/**
 * Thread 03
 * 01. t.join() : join the thread to main thread, so that the main thread wait for the completion of t before
 *      ending itself.
 * 
 * 02. t.joinable() : check if we can join this thread to main thread or not.
 * 
 * 03. t.detach() : detaches the thread from main thread. This leads to condition where main thread won't wait
 *     for t's completion. t is independent from main thread. We need to make sure that t does not access any memory that
 *     is freed after main ends.
 * 
 * For each thread either the join, or detach has to be called, if not then when the thread exits it check if
 * t1 and t2 are joinable or not, if they are joinable. Main thread calls std::terminate, which lead to program
 * crashing or exiting abnormally.
 */
#include <iostream>
#include <thread>
#include <chrono>
#include <unistd.h>

void threadCallableFunction1(int secs)
{
    std::cout<<"Inside Thread w Function : "<<std::this_thread::get_id()<<std::endl;
    std::cout<<"\tSleep for : "<<secs<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(secs));
    std::cout<<"Sleep Done : "<<secs<<std::endl;
}
int main()
{
    std::cout<<"Inside Main Thread : "<<std::this_thread::get_id()<<std::endl;

    std::thread t1(threadCallableFunction1, 5);
    std::thread t2(threadCallableFunction1, 5);

    // std::this_thread::sleep_for(std::chrono::seconds(10));
    // sleep(10);

    std::cout<<"Main Thread Complete"<<std::endl;
    return  0;
}