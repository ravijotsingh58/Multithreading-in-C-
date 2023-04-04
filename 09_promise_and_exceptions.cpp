//in this code, we will demonstarte how to set exception with future when exception is thrown from function
//we can set exception in promise with set_exception method. when exception is thrown, get() method of future 
//rethrows that same exception and we can catch it in main thread and handle it.

#include<iostream>
#include<iomanip>
#include<future>
#include<exception>

using namespace std;

double leibnizPieCalc(int terms) {

    if(terms < 1){
        throw runtime_error{"Terms should be greater than or equal to 1"};
    }

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

    auto pieCalc = [&](int terms) {

        try{

            auto result = leibnizPieCalc(terms);
            pi_promise.set_value(result);
        }catch(const std::exception& e){
            pi_promise.set_exception(current_exception()); //current exception() will captures the current exception object
            //and creates the std::exception_ptr pointer that holds the copy or reference to that exception object
        }
    };

    thread th1{ pieCalc, 0 }; //passing 0 so that it throws exception

    future<double> pi_future = pi_promise.get_future();

    try{

        cout << setprecision(15) << pi_future.get() << endl; //this is blocking call and waits for result to finish
    }catch(const std::exception& e){
        cout<<"Exception Thrown: "<<e.what()<<endl;
    }

    th1.join();

    return 0;
}

