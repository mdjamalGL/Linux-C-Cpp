/**
 * Semaphore : Allows to manage limited number of resources and simple signalling.
 * Counting Semaphore : This allows managing resources with n of its instance, thus n threads can acquire the 
 *      semaphore at a time.
 *      we can have, acquire, release, try_acquire, try_acquire_for, and try_acquire_until
 */

 #include <iostream>
 #include <thread>
 #include <semaphore>
 #include <chrono>
 
 int amount = 0;
 const int leastMaxValue = 2;

 std::counting_semaphore<leastMaxValue> csmp(leastMaxValue);

 void threadCallableFunction(int sec)
 {
    std::cout<<"Inside Thread Function  with ID : "<<std::this_thread::get_id()<<std::endl;

    if(csmp.try_acquire())
    {
        std::cout<<"Acquiring Semaphore : "<<std::this_thread::get_id()<<std::endl;

        for(int i = 0 ; i < 100000; i++)
        amount++;

        std::cout<<"Simulating Work : "<<sec<<" seconds"<<std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(sec));
        std::cout<<"Simulating Work ends"<<std::endl;

        std::cout<<"Releasing Semaphore : "<<std::this_thread::get_id()<<std::endl;
        csmp.release();
    }
    else
    {
        std::cout<<"Could not Acquire Semaphore : "<<std::this_thread::get_id()<<std::endl;
    }
 }
 
 int main()
 {
     std::cout<<"Inside Main Thread : "<<std::this_thread::get_id()<<std::endl;
 
     int sleep_sec_1 = 3;
     int sleep_sec_2 = 2;
     int sleep_sec_3 = 2;
 
     std::thread t1(threadCallableFunction, sleep_sec_1);
     std::thread t2(threadCallableFunction, sleep_sec_2);
     std::thread t3(threadCallableFunction, sleep_sec_3);
 
     if(t1.joinable())
     t1.join();
 
     if(t2.joinable())
     t2.join();
    
     if(t3.joinable())
     t3.join();

     std::cout<<"Amount : \n\tActual : "<<amount<<"\n\tExpected : "<<2000000<<std::endl;
     std::cout<<"Main Thread Complete"<<std::endl;
 }