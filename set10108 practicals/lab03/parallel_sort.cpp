#include <iostream>
#include <omp.h>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>
#include <thread>

std::vector<unsigned int> generate_values(size_t size) {
    // create random engine
    std::random_device r;
    std::default_random_engine e(r());
    // generate numbers
    std::vector<unsigned int> data;
    for(size_t i = 0; i < size; ++i)
        data.push_back(e());

    return data;
}

void parallel_sort(std::vector<unsigned int>& values) {
    // get num of threads
    auto num_threads = std::thread::hardware_concurrency();
    //std::cout << "Number of threads: " << num_threads << std::endl;
    // get number of elements in vector
    auto n = values.size();
    // declare needed variables
    int i, tmp, phase;
    // declare parallel stuff
#pragma omp parallel num_threads(num_threads) default(none) shared(values, n) private(i, tmp, phase)
    for (phase = 0; phase < n; ++phase) {
        // determine current phase of sorting
        if(phase % 2 == 0) {
            // each thread jumps forward 2, no conflict
#pragma omp for
            for (i = 1; i < n; i += 2) {
                // check if values should be swapped
                if(values[i-1] > values[i]) {
                    //swap values
                    tmp = values[i - 1];
                    values[i - 1] = values[i];
                    values[i] = tmp;
                }
            }
        } else {
            //each thread jumps forward 2, no conflict
#pragma omp for
            for (i = 1; i < n; i += 2) {
                // check if values should be swapped
                if(values[i] > values[i+1]) {
                    // swap
                    tmp = values[i + 1];
                    values[i + 1] = values[i];
                    values[i] = tmp;
                }
            }
        }
    }
}

int main() {
    // create results file
    std::ofstream results("bubble_parallel.csv", std::ofstream::out);

    for(size_t size = 8; size <= 16; ++size) {
        // show data size
        results << pow(2, size) << ", ";
        // gather 100 results
        for(size_t i = 0; i < 100; ++i) {
            // generate a vector of random values
            std::cout << "Generating " << i << " for " << pow(2, size) << " values " << std::endl;
            auto data = generate_values(static_cast<size_t>(pow(2, size)));
            // sort and time
            std::cout << "Sorting..." << std::endl;
            auto start = std::chrono::system_clock::now();
            parallel_sort(data);
            auto end = std::chrono::system_clock::now();
            auto total = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            // output time
            results << total << ",";
        }
        results << std::endl;
    }
    results.close();

    return 0;
}