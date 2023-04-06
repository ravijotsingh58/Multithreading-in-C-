//std::async is a function provided by the C++ Standard Library that allows for asynchronous execution of a function or callable object. When std::async is called, it creates a new thread to execute the function or callable object asynchronously, 
//and returns an std::future object that can be used to obtain the result of the function once it has completed execution. 
//The std::future object can also be used to check if the function has finished 
//executing, and to retrieve any exceptions that were thrown during execution.
//if we pass launch::deferred as first argument, then it will block execution of
//thread till we call get() method on returned future. If we pass launch::async then
//it will execute thread even without calling get() method on returned future. However,
//if we pass launch::async as first arguments and invoke multiple threads on same function
//or callable object, then it is required that we capture returned futured from thread
//invoking call. Otherwise both threads will execute synchronously rather than asynchronoulsy.
#include<iostream>
#include<chrono>
#include<thread>
#include<future>

using namespace std;

void func(int id){
    
    for(int i = 0;i<5;i++){
        
        cout<<"Running: "<<id<<endl;
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int func2(int id){
        
    for(int i = 0;i<5;i++){
        
        cout<<"Running: "<<id<<endl;
        this_thread::sleep_for(chrono::seconds(1));
    }

    return id*7;
}

int main(int argc, char** argv){
    
    async(launch::async, func, 0);
    async(launch::async, func, 1);//these both calls will execute synchronously rather than asynchronously as we are not
    //caturing returned future.

    future<void> f1 = async(launch::deferred, func, 0);
    future<void> f2 = async(launch::deferred, func, 1); 
    //these both will stop thread executing till we call get() method on each of future (f1 and f2).
    f1.get(), f2.get(); //now both threads will execute perfectly but asynchronously

    future<int> f3 = async(launch::async, func2, 3);
    future<int> f4 = async(launch::async, func2, 4);
    //these 2 threads will execute asynchronouly
    cout<<f3.get()<<endl;
    cout<<f4.get()<<endl;

    return 0;

}
