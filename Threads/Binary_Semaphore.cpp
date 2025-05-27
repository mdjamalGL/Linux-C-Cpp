/**
 * Semaphore : Allows to manage limited number of resources and simple signalling.
 * Binary Semaphore : A specialization of Counting Semaphore, allows only one thread access to the
 *      Resource at a time.
 */

#include <iostream>
#include <thread>
#include <semaphore>
#include <unistd.h>
#include <chrono>

int amount = 0;
std::binary_semaphore bsmp(0);
void threadCallableFunctionA(int sec)
{
    std::cout<<"Inside Thread Function  with ID : "<<std::this_thread::get_id()<<std::endl;

    for(int i = 0 ; i < 100000; i++)
    amount++;

    std::cout<<"Simulating Work : "<<sec<<" seconds"<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(sec));
    std::cout<<"Simulating Work ends"<<std::endl;

    std::cout<<"Releasing Semaphore : "<<std::this_thread::get_id()<<std::endl;
    bsmp.release();
}

void threadCallableFunctionB(int sec)
{
    std::cout<<"Inside Thread Function  with ID : "<<std::this_thread::get_id()<<std::endl;

    bsmp.acquire();
    std::cout<<"Acquired Semaphore : "<<std::this_thread::get_id()<<std::endl;

    for(int i = 0 ; i < 100000; i++)
    amount++;

    std::cout<<"Simulating Work : "<<sec<<" seconds"<<std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(sec));
    std::cout<<"Simulating Work ends"<<std::endl;

    std::cout<<"Releasing Semaphore : "<<std::this_thread::get_id()<<std::endl;
    bsmp.release();
}

int main()
{
    std::cout<<"Inside Main Thread : "<<std::this_thread::get_id()<<std::endl;

    int sleep_sec_1 = 3;
    int sleep_sec_2 = 2;

    std::thread t1(threadCallableFunctionA, sleep_sec_1);
    std::thread t2(threadCallableFunctionB, sleep_sec_2);

    if(t1.joinable())
    t1.join();

    if(t2.joinable())
    t2.join();

    std::cout<<"Amount : \n\tActual : "<<amount<<"\n\tExpected : "<<2000000<<std::endl;
    std::cout<<"Main Thread Complete"<<std::endl;
}