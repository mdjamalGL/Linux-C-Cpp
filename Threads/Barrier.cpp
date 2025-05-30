/**
 * 
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <barrier>
#include <vector>

int amount = 0;
std::mutex mtx;

void threadCallableFunction(int id, std::barrier<> &brr)
{
    std::cout<<"Inside thread : "<<id<<std::endl;

    {
        std::lock_guard<std::mutex> lguard(mtx);
        for(int i = 0 ; i < 1000000; i++)
        {
            amount++;
        }
    }

    std::cout<<"\tThread "<<id<<" waiting"<<std::endl;
    brr.arrive_and_wait();
    std::cout<<"\tThread "<<id<<" continues"<<std::endl;

}
int main()
{
    std::cout<<"Inside Main Thread"<<std::endl;
    std::barrier brr(3);

    std::vector<std::thread> threads;

    for(int i = 0 ; i < 3; i++)
    {
        threads.emplace_back(threadCallableFunction, i+1, std::ref(brr));
    }

    for(auto &t : threads)
    {
        t.join();
    }

    std::cout<<"Amount : \n\tActual : "<<amount<<"\n\t Expected : "<<3000000<<std::endl;
    std::cout<<"Main Thread Complete"<<std::endl;
    return 0;
}