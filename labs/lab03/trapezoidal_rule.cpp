#include <omp.h>
#include <iostream>
#include <functional>
#include <memory>
#include <cmath>
#include <thread>

void trap(std::function<double(double)> f, double start, double end, size_t iterations, std::shared_ptr<double> p) {
    // get thread number
    auto my_rank = omp_get_thread_num();
    // get number of threads
    auto thread_count = omp_get_num_threads();
    // calculate iteration slice size
    auto slice_size = (end - start) / iterations;
    // calculate num of iterations per thread
    auto iterations_thread = iterations / thread_count;
    // calculate this thread's start point
    auto local_start = start + ((my_rank * iterations_thread) * slice_size);
    // calculate this thread's end point
    auto local_end = local_start + iterations_thread * slice_size;
    // calculate initial result
    auto my_result = (f(local_start) + f(local_end)) / 2.0;

    // make sure x is preserved during iterations by predeclaring
    double x;
    // sum each iteration
    for(size_t i = 0; i <= iterations_thread - 1; ++i) {
        // calculate next slice to calculate
        x = local_start + i * slice_size;
        // add to current result
        my_result += f(x);
    }
    // multiply the result by the slice size
    my_result *= slice_size;

    // add to shared data
    // NOTE: critical means only 1 thread can do this at any time
#pragma omp critical
    *p += my_result;
}

int main() {
    // declare shared result
    auto result = std::make_shared<double>(0.0);
    // define start and end values
    auto start = 0.0;
    auto end = 3.14159265359; // pi
    // number of trapezoids to generate
    auto trapezoids = static_cast<size_t>(pow(2, 24));
    // get number of threads
    auto thread_count = std::thread::hardware_concurrency();

    // function to calculate integral of; cos
    auto f = [](double x) { return sin(x); };

    // run trap() in parallel
#pragma omp parallel num_threads(thread_count)
    trap(f, start, end, trapezoids, result);

    // output result
    std::cout << "Using " << thread_count << " threads" << std::endl;
    std::cout << "Using " << trapezoids << " trapezoids. " << std::endl;
    std::cout << "Estimated integral of function " << start << " to " << end << " = " << *result << std::endl;

    return 0;
}