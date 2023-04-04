//Here we will calculate pie with leibniz formual. However, we will create this process more efficient later
//by using multithreading
//leibniz formula to calulate pie = 1 - (1/3) + (1/5) - (1/7) + (1/9)... = (pi/4)
//=> 4 * ( 1 - (1/3) + (1/5) - (1/7) + (1/9) ...) = pi
//=> 4 * sumation(k >= 0) pow(-1, k) (1/ (2*k + 1)) = pi

#include<iostream>
#include<iomanip>

using namespace std;

double leibnizPieCalc(int terms){

    double sum = 0.0;
    
    for(int i = 0;i < terms; i++){
        int sign = (i&1) ? -1 : 1; //pow(-1, i);
        double term = (1.0)/(i * 2 + 1);
        sum += (sign * term);
    }

    return sum * 4;
}

int main(int argc, char** argv){
    
    cout<<setprecision(15)<<leibnizPieCalc(1E5)<<endl;
}
