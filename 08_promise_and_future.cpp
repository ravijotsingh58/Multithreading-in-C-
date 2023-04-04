//Promise and Future are worrk together like producer and consumer. Promise sets the result from thread and 
//future get it (or consumes that result). get() result of future is blocking function. That means it will wait
//to execute promise.set_value() function to execute and after that only, it will return the final result. so because
//of this. It dosen't matter whether we use thread.join() fucntion before future.get() call or aftert it. because ultimately
//future.get() will wait for result and we will get final correct result in both scenarios.
//future is the thing that we get immidiately from promise with promise.get_future call. But future have to wait to get
//the final result with future.get() method. 

#include<iostream>
#include<iomanip>
#include<future>

using namespace std;

double leibnizPieCalc(int terms) {

    double sum = 0.0;

    for (int i = 0; i < terms; i++) {
        int sign = (i & 1) ? -1 : 1; //pow(-1, i);
        double term = (1.0) / (i * 2 + 1);
        sum += (sign * term);
    }

    return sum * 4;
}

int main(int argc, char** argv) {

    promise<double> pi_promise;

    auto pieCalc = [&](int terms){
        
        auto result = leibnizPieCalc(terms);
        pi_promise.set_value(result);
    };

    thread th1{pieCalc, 1E8};

    future<double> pi_future = pi_promise.get_future();
    
    cout << setprecision(15) << pi_future.get() << endl; //this is blocking call and waits for result to finish

    th1.join();

    return 0;
}
