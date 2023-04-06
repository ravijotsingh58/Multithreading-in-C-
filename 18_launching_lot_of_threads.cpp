//Here we will launch lots of threads to execute parallely. we will use std::async to run them asynchronouly.
#include<iostream>
#include<thread>
#include<future>
#include<chrono>
#include<mutex>
#include<vector>

using namespace std;

mutex g_mtx;

int work(int id){
    
    unique_lock<mutex> lock(g_mtx);
    cout<<"Starting: "<<id<<endl;
    lock.unlock();
    this_thread::sleep_for(chrono::seconds(3));//some heavy work
    return id;
}

int main(int argc, char** argv){
    
    vector<shared_future<int>> futures; //as std::future is not copyable, we have to use shared_future that is copyable.
    //shared_future hads constructor that accept std::future asgument. So we van push returned futures in vector.

    for(int i = 0;i<thread::hardware_concurrency();i++){
        
        shared_future<int> f = async(launch::async, work, i);
        //if we not store future references in vector, that future will get out of scope after end of loop and thread will
        //exeute synchronouly rather than asynchronouly.
        futures.push_back(f);
    }

    for(auto x : futures){
        
        //getting returned values of futures
        cout<<x.get()<<endl;
    }

    return 0;
}

