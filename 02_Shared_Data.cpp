#include<iostream>
#include<thread>
#include<atomic>

using namespace std;
/*
one thread is trying to increment the calue whereas another thread is tying to decrement the value
std::atomic<> template is used to provide atomic operations on shared variable that can be accessed by multiple
thread concurrently. If we not use atomic<> here then count variable final value will be undefined.
But after using atomic<> here, count variable's final value is 0.
Thats a general fix that works for this case. However, this is not a general fix for general problem i.e what happens
when multiple threads   works on shared data
*/

int main(int argc, char** argv){
    
    atomic<int> count = 0;
    const int ITERS = 1E6;
    thread th1{[&](){
        for(int i = 1; i<= 1E6; i++)
            ++count;
    }};

    thread th2{[&](){
        for(int i = 1;i<=1E6;i++){
            --count;
        }
    }};

    th1.join();
    th2.join();

    cout<<"Final count: "<<count<<endl;

    return 0;
}
