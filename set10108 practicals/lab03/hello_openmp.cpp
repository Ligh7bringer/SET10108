#include <iostream>
#include <omp.h>

constexpr int THREADS = 10;

void hello() {
    // get thread number
    auto my_rank = omp_get_thread_num();
    // get number of threads used
    auto thread_count = omp_get_num_threads();
    // display message
    std::cout << "Hello from thread " << my_rank << " of " << thread_count << std::endl;
}

int main() {
    // run hello
#pragma omp parallel num_threads(THREADS)
    hello();

    return 0;
}
