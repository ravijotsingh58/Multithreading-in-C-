//In this example, we will overload callable object. Callable object is when we overload calling operator '()' for the object
//we have to pass object as reference in thread construct rather than as value because class's copy constructor is got
//deleted. 

#include<iostream>
#include<thread>
#include<mutex>

using namespace std;

class App{
public:
    void operator()(){        
        for (int i = 1; i <= 1E6; i++) {

            lock_guard<mutex> guard(mtx);
            count += step;
        }
    }

    int getCount() const{
        
        return count;
    }

    void setstep(int _step){
        step = _step;
    }

private:
    mutex mtx;
    int step{1}, count;
};

int main(int argc, char** argv) {

    App app;

    thread th1{ref(app)};
    thread th2{ref(app)};

    //as one thread is trying to increase the value of count and other is decreasing it, the final value
    //of count should be 0

    th1.join(), th2.join();


    cout << "Final count: " << app.getCount() << endl;

    return 0;
}
