//Here we will check how many threads we can genuinly run at the same time. For that we will use static method of thread class
//hardware_concurrency.

#include<iostream>
#include<thread>
using namespace std;

int main(int argc, char** argv){
    
    cout<<thread::hardware_concurrency()<<endl;

    return 0;
}
