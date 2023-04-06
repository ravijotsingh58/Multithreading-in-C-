//Here we will use asyn to distribute pie calculation ntask to different threads. Each thread will calculate their part
//and at the end we will add them all to get a final value 
#define _USE_MATH_DEFINES
#include<iostream>
#include<thread>
#include<future>
#include<chrono>
#include<mutex>
#include<vector>
#include<iomanip>
#include<cmath>

using namespace std;

mutex g_mtx;

double leibnizPieCalc(int terms, int start, int skip) {

    double sum = 0.0;

    for (int i = start; i < terms; i+=skip) {
        int sign = (i & 1) ? -1 : 1; //pow(-1, i);
        double term = (1.0) / (i * 2 + 1);
        sum += (sign * term);
    }

    return sum * 4;
}

int main(int argc, char** argv) {

    vector<shared_future<double>> futures; //as std::future is not copyable, we have to use shared_future that is copyable.
    //shared_future hads constructor that accept std::future asgument. So we van push returned futures in vector.
    
    int CONCURRENCY = thread::hardware_concurrency();

    for (int i = 0; i < CONCURRENCY; i++) {

        shared_future<double> f = async(launch::async, leibnizPieCalc, 1E8, i, CONCURRENCY);
        //if we not store future references in vector, that future will get out of scope after end of loop and thread will
        //exeute synchronouly rather than asynchronouly.
        futures.push_back(f);
    }
    
    double sum = 0.0;

    for (auto x : futures) {

        //getting returned values of futures
        sum += x.get();
    }

    //cout<<"PI:  "<<M_PI<<endl;
    cout<<setprecision(15)<<"SUM: "<<sum<<endl;

    return 0;
}
