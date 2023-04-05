//we will use condition variable to make one thread to wait for certain condition to fulfill to continue
//its task. Condition variables work with mutex, so we have to declare one mutex as well. here we will use unique lock rather
//than lock guard because we use unlock method to unlock mutex. lock guard does not have unlock() method. 

#include<iostream>
#include<thread>
#include<condition_variable>
#include<mutex>
#include<chrono>


using namespace std;

int main(int argc, char** argv) {

    condition_variable condvar;
    mutex mtx;
    bool ready{ false };

    thread t1{ [&]() {
        this_thread::sleep_for(chrono::milliseconds(2000));
        cout<<"Thread t1 acquiring lock"<<endl;
        unique_lock<mutex> lock(mtx);
        cout<<"Thread t1 acquired lock"<<endl;
        ready = true;
        lock.unlock();
        cout<<"Thread t1 released lock..Notifying"<<endl;
        condvar.notify_one();
    } };
    
    cout<<"Main thread acquiring lock"<<endl;
    unique_lock<mutex> lock(mtx);
    
    cout<<"Main thread acquired lock--waiting"<<endl;
    //rather than using while condition to check whether ready is true
    //we can use predicate in wait function, and only if that predicate returns true, we can move forward and execute
    //otherwise it will go back and wait again;
    condvar.wait(lock, [&](){return ready;});

    t1.join();
    
    cout<<"Main thread finished"<<endl;
    cout << "Ready state: " << ready << endl;

    return 0;
}
