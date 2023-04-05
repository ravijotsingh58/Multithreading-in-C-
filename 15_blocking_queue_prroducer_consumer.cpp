//Here we will implement blocking queue. Blocking queue meand push or pop operation on queue 
//will be blocked untile some condition. here in this case, push will be blocked if queue has max size
//elements and will wait for pop operation to make space to push more element. Same for pop, it will be
//blocked until queue is empty and will be blocked until some element is pushed in queue. 

#include<iostream>
#include<queue>
#include<thread>
#include<condition_variable>
#include<mutex>

using namespace std;

template<typename T>
class blocking_queue{
private:
    mutex mtx;
    condition_variable condvar;
    queue<T> que;
    int _max_size{};

public:
    blocking_queue(int max_size):_max_size{max_size}{}

    void push(T elem){
        
        unique_lock<mutex> lock(mtx);
        condvar.wait(lock, [this](){return que.size() < _max_size;});
        que.push(elem);
        lock.unlock();
        condvar.notify_one(); //notifying consumer thread
    }

    void pop(){

        unique_lock<mutex> lock(mtx);
        condvar.wait(lock, [this](){return !que.empty();});
        que.pop();
        lock.unlock();
        condvar.notify_one(); //notifying producer thread
    }

    T front(){
        
        unique_lock<mutex> lock(mtx);
        condvar.wait(lock, [this](){return !que.empty();});

        auto elem = que.front();
        //here notifying is not required

        return elem;
    }

    size_t size(){
        
        lock_guard<mutex> lock(mtx);
        return que.size();
    }
};

int main(int argc, char** argv){
    
    blocking_queue<int> que(5);
    /*thread t1{&blocking_queue<int>::push, &que, 7};
    //here we are passing on which object we want to invoke class's method function
    //here we are invoking push on que object. passing their addresses as arguments to thread constructor
    cout<<que.front()<<endl;
    thread t2{&blocking_queue<int>::pop, &que};*/
    thread t1{[&](){
        for(int i = 0;i<10;i++){
            cout<<"Producing: "<<i+1<<" queue size = "<<que.size()<<endl;
            que.push(i+1);
        }
    }};

    thread t2{[&](){
        for(int i = 0;i<10;i++){
            auto elem = que.front();
            que.pop();
            cout<<"Consumed: "<<elem<<endl;
        }
    }};
    t1.join(), t2.join();

    return 0;
}
