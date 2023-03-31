#include<iostream>
#include<thread>
#include<mutex>
using namespace std;

/*
in C++, if we pass arguments directly without std::ref or std::reference_wrapper, the arguments will be copied 
into new thread rather than being passed by reference. when variable is copied into new thread, it creates a new
instance of the variable in the new thread's memory space. This means that any modifications made to the variable
in the new thread will not be reflected in the original variable in the main thread.
further, if critical throws an exception for some reason, them mutex will not be get unlocked. 
So for that reason we should use unique locks and lock guards 
*/

void performIncr(int& count, mutex& mtx){
    
    for(int i = 1;i <= 1E6; i++){
        mtx.lock();
        ++count;
        mtx.unlock();
    }
}

int main(int argc, char** argv){

    int count = 0;
    mutex mtx;
    
    thread th1{performIncr, ref(count), ref(mtx)};
    thread th2{performIncr, ref(count), ref(mtx)};

    th1.join(); th2.join();

    cout<<"Final count: "<<count;
    return 0;
}
