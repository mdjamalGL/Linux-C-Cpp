/**
 * Thread 01
 * 01. std::thread t : make a new thread from the current thread. It needs a callable object
 *     and the object's argument as the param for thread creation constructor.
 * 
 * 02. t.join() : join the thread to main thread, so that the main thread wait for the completion of t before
 *      ending itself.
 * 
 * 03. std::this_thread::get_id() : get the thread id of current thread.
 */
#include <iostream>
#include <thread>

void threadCallableFunction1(std::string param)
{
    std::cout<<"Inside Thread w Function : "<<std::this_thread::get_id()<<std::endl;
    std::cout<<"\tParam : "<<param<<std::endl;
}
int main()
{
    std::cout<<"Inside Main Thread : "<<std::this_thread::get_id()<<std::endl;

    std::thread t1(threadCallableFunction1, std::string("T1"));
    std::thread t2([](std::string param)
                    {
                        std::cout<<"Inside Thread w Lambda : "<<std::this_thread::get_id()
                                 <<"\n\t"<<param<<std::endl;
                    }, std::string("Param : T2"));

    t1.join();
    t2.join();

    std::cout<<"Main Thread Complete"<<std::endl;
    return  0;
}