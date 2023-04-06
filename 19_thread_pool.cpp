//Here we will launch multiple threads to run asynchronously using async and blocking queue. However, we will bound that 
//only certain number can be executed
//at a given time. Further, suppose we set limit as 2, then only 3 threads will be executed at a given time but only 2 threds
//can be pushed to a queue. so till we have not pop them out, we will not be able to execute more threads and push other
//threads to blocking_queue.
#include<iostream>
#include<thread>
#include<future>
#include<chrono>
#include<mutex>
#include<queue>
#include<condition_variable>
#include<cmath>

#define MAX_THREADS 20

using namespace std;

mutex g_mtx;

template<typename T>
class blocking_queue {
private:
    mutex mtx;
    condition_variable condvar;
    queue<T> que;
    int _max_size{};

public:
    blocking_queue(int max_size) :_max_size{ max_size } {}

    void push(T elem) {

        unique_lock<mutex> lock(mtx);
        condvar.wait(lock, [this]() {return que.size() < _max_size; });
        que.push(elem);
        lock.unlock();
        condvar.notify_one(); //notifying consumer thread
    }

    void pop() {

        unique_lock<mutex> lock(mtx);
        condvar.wait(lock, [this]() {return !que.empty(); });
        que.pop();
        lock.unlock();
        condvar.notify_one(); //notifying producer thread
    }

    T front() {

        unique_lock<mutex> lock(mtx);
        condvar.wait(lock, [this]() {return !que.empty(); });

        auto elem = que.front();
        //here notifying is not required

        return elem;
    }

    size_t size() {

        lock_guard<mutex> lock(mtx);
        return que.size();
    }
};

int work(int id){
    
    unique_lock<mutex> lock(g_mtx);
    cout<<"Sending: "<<id<<endl;
    lock.unlock();
    int seconds = static_cast<int>((5.0 * rand())/RAND_MAX + 3); //getting random number of seconds. rand()/RAND_MAX will generate 0 or 1.
    //because RAND_MAX is the maximum greates random number that rand() can generate.
    this_thread::sleep_for(chrono::seconds(seconds));//some heavy work
    return id;
}

int main(int argc, char** argv){
    
    blocking_queue<shared_future<int>> futures(2); //this number id one less that total number of threads that we
    //want to execute at a single time. so here, 3 threads will be executed at a single time. 
    //as std::future is not copyable, we have to use shared_future that is copyable.
    //shared_future hads constructor that accept std::future asgument. So we van push returned futures in vector.
    
    thread t1{[&](){

        for(int i = 0;i<MAX_THREADS;i++){
        
            shared_future<int> f = async(launch::async, work, i);
            //if we not store future references in vector, that future will get out of scope after end of loop and thread will
            //exeute synchronouly rather than asynchronouly.
            futures.push(f);
         }
    }}; //we are executing push operation in separate thread

    for(int i = 0;i<MAX_THREADS;i++){
        
        //getting returned values of futures
        shared_future<int> f = futures.front();
        int num = f.get(); //getting return value with get()
        futures.pop();
        cout<<"Receiving: "<<num<<endl;
    } //executing pop() operating in separate thread i.e. main thread

    t1.join();

    return 0;
}

