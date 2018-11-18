#include <omp.h>
#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>

// relies on i to determine amount of work
double f(unsigned int i) {
    // calculate start and end
    auto start = i * (i + 1) / 2;
    auto end = start + i;
    // return value variable
    auto result = 0.0;

    for (int j = start; j < end; ++j) {
        result += sin(j);
    }

    return result;
}

int main() {
    // get number of threads
    auto thread_count = std::thread::hardware_concurrency();
    // define number of iterations
    auto n = static_cast<size_t>(pow(2, 14));
    // declare sum variable
    auto sum = 0.0;

    // get start time
    auto start = std::chrono::system_clock::now();
#pragma omp parallel for num_threads(thread_count) reduction(+:sum) schedule(static, 1)
    for (int i = 0; i < n; ++i) {
        sum += f(i);
    }

    // get end time
    auto end = std::chrono::system_clock::now();
    auto total = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Total time: " << total << "ms" << std::endl;

    return 0;
}