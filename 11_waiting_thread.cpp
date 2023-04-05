//Main thread is waiting for another thread to complete some condition to fulfill
//here we have used naive approach to fulfill this scenario
#include<iostream>
#include<thread>
#include<chrono>
#include<atomic>

using namespace std;

int main(int argc, char** argv){
    
    atomic<bool> ready{false}; //so that every that has access to same ready variable atomically
    thread t1{[&](){
        this_thread::sleep_for(chrono::milliseconds(2000));
        ready = true;
    }};

    t1.join();

    while(!ready){

        this_thread::sleep_for(chrono::milliseconds(100));
    }

    cout<<"Ready value: "<<ready;

    return 0;
}
