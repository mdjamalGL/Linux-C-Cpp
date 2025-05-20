/**
 * Mutex 02
 * 01. std::recurive_mutex : This is used for securing critical section and avoid race conditions.
 *     It provides exclusive, and recursive ownership. i.e. same thread can acquire an already 
 *     acquired mutex over and over again. It will have to release it as many time as the lock was
 *     acquired.
 *     Used in cases of Recursion as shown below.
 * 02. we use lock() and unlock() methods, with the mutex to restrict the access of critical section.
 * 
 */
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <unistd.h>

long long int amount = 0;
std::recursive_mutex rmtx;

void threadCallableRecursiveFunction(int counter)
{
    rmtx.lock();
    std::cout<<"\tlock acquired : "<<counter<<std::endl;
    
    if(counter > 0)
    threadCallableRecursiveFunction(counter-1);

    std::cout<<"\tlock freed : "<<counter<<std::endl;
    rmtx.unlock();
}

int main()
{
    std::cout<<"Inside Main Thread : "<<std::this_thread::get_id()<<std::endl;

    std::thread t1(threadCallableRecursiveFunction, 5);

    if(t1.joinable())
    t1.join();

    std::cout<<"Main Thread Complete"<<std::endl;
    return  0;
}