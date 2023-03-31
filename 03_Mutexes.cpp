#include<iostream>
#include<thread>
#include<mutex>

using namespace std;
/*
In this case, both threads are tying to increment shared count variable 1E3 times. so we have used mutex
to perform increment operation in critical section so that only one thread is allowed to access and increment 
the variable at a time. so final value will be 2E3. If we do not use mutex here, then the final value of count is undefined.
we should take note that critical section should be minimum as possible and only shared variable operations
should be placed inside critical section. we should not put code between lock() and unlock() where it is not required. 
*/

int main(int argc, char** argv){
    
    int count = 0;    
    const int ITERS = 1E3;

    mutex mtx;

    auto func =  [&](){
        for(int i = 1; i<= ITERS; i++){
            mtx.lock();
            ++count;
            mtx.unlock();
            /*
            here we should not place whole for loop between lock() and unlock() as it is not required.
            we should make critical section minimum as possible where it is required.
            */
        }
    };
    thread th1{func};

    thread th2{func};

    th1.join();
    th2.join();

    cout<<"Final count: "<<count<<endl;

    return 0;
}
