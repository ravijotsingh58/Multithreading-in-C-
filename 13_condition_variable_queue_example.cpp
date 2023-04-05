//here we will try producer consumer problem withe the help of queue and condition variable. Producer will push random number
//to queue whereas producer will pop them from queue and print them
#include<iostream>
#include<queue>
#include<condition_variable>
#include<mutex>
#include<thread>
#include<random> //to generate random numbers

using namespace std;

const int lim{10};

void producer(queue<long>& que, bool& done, mutex& mtx, condition_variable& condition, int& count){
    
    while(count < lim){
        this_thread::sleep_for(chrono::seconds(2));
        unique_lock<mutex> lock{mtx};
        std::random_device rd;
        std::mt19937 mt{rd()};
        std::uniform_int_distribution<long> dist{123, 99936373};
        long num = dist(mt);
        cout<<"Producer sent: "<<num<<endl;
        que.push(num);
        count++;
        if(count == lim){
            done = true;
        }
        lock.unlock();
        condition.notify_one();
    }
}

void consumer(queue<long>& que, bool& done, mutex& mtx, condition_variable& condition){
    
    while(!done){
        unique_lock<mutex> lock{mtx};
        condition.wait(lock, [&]{return que.size();});
        long num = que.front();
        que.pop();
        cout<<"Consumer get: "<<num<<endl;
    }
}

int main(int argc, char** argv){
    
    queue<long> que;
    mutex mtx;
    condition_variable condition;
    bool done{false};
    int count{0};

    thread t1{producer, ref(que), ref(done), ref(mtx), ref(condition), ref(count)};
    thread t2{consumer, ref(que), ref(done), ref(mtx), ref(condition)};

    t1.join(), t2.join();

    return 0;
}
