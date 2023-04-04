//Here i will demonstarte packaged tasks. it workks similar to promise and future. However, we do not
//have to manage promise directly. in this way, management promise and future process becomes easy.
//further, we don't have to use set_value and set_exception() separately as that will be managed by 
// packaged_task automastically.  

#include<iostream>
#include<iomanip>
#include<future>
#include<exception>

using namespace std;

double leibnizPieCalc(int terms) {

    if (terms < 1) {
        throw runtime_error{ "Terms should be greater than or equal to 1" };
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

    packaged_task<double(int)> pi_task{leibnizPieCalc};

    future<double> pi_future = pi_task.get_future(); //here we have to get future with get_future() before
    //spawning the thread. otherwise it will not work correctly

    thread th1{ move(pi_task), 0 }; //passing 0 so that it throws exception

    try {

        double result = pi_future.get();

        cout << setprecision(15) << result << endl; //this is blocking call and waits for result to finish
    }
    catch (const std::exception& e) {
        cout << "Exception Thrown: " << e.what() << endl;
    }

    th1.join();

    return 0;
}
