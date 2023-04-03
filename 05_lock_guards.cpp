//As we have discussed previously that using mutexes is not really ideal because some exception might be thrown
//in critical section and it will never unlocked. So we prefer to use RAII. 
//when we declare a lock_guard and we pass mutex (of any kind) to it, it will actually acquire the lock to that mutex 
//if it can. Otherwise it gonna wait until it can acquire the lock. So basically it gonna same as
//locking a mutex. But when it goes out of scope (at scope ends or exception thrown), it will release the lock.

#include<iostream>
#include<thread>
#include<mutex>

using namespace std;

void incrFunc(int& count, mutex& mtx, const int step){
    
    for(int i = 1; i<= 1E6; i++){
        
        lock_guard<mutex> guard(mtx);
        count+=step;
    }
}

int main(int argc, char** argv){
    
    int count = 0;
    mutex mtx;

    thread th1{incrFunc, ref(count), ref(mtx), 1};
    thread th2{incrFunc, ref(count), ref(mtx), -1};

    //as one thread is trying to increase the value of count and other is decreasing it, the final value
    //of count should be 0

    th1.join(), th2.join();

    
    cout<<"Final count: "<<count<<endl;

    return 0;
}
