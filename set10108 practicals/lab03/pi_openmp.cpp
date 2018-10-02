#include <omp.h>
#include <iostream>
#include <thread>
#include <cmath>

int main() {
    // get number of supported threads
    auto num_threads = std::thread::hardware_concurrency();

    // number of iterations to perform
    const int n = static_cast<int>(pow(2, 30));
    // factor value
    double factor = 0.0;
    // calculated pi
    double pi = 0.0;

    // parallel for loop
#pragma omp parallel for num_threads(num_threads) reduction(+:pi) private(factor)
    for(int k = 0; k < n; ++k) {
        // determine sign of factor
        if(k % 2 == 0)
            factor = 1.0;
        else
            factor = -1.0;
        // add this iteration value to pi sum
        pi += factor / (2.0 * k + 1);
    }

    // get final value of pi
    pi *= 4.0;

    // show more precision
    std::cout.precision(std::numeric_limits<double>::digits10);
    std::cout << "pi = " << pi << std::endl;

    return 0;
}
