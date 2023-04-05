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
        unique_lock<mutex> lock(mtx);
        ready = true;
        lock.unlock();
        condvar.notify_one();
    } };

    unique_lock<mutex> lock(mtx);
    while (!ready) {

        condvar.wait(lock);// wait will release the lock if not notified yet
    }

    t1.join();

    cout << "Ready state: " << ready << endl;

    return 0;
}
