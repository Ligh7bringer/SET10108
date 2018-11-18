#include <iostream>
#include <omp.h>
#include <vector>
#include <random>
#include <chrono>
#include <fstream>

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

void bubble_sort(std::vector<unsigned int>& values) {
    for(auto count = values.size(); count >= 2; --count) {
        for(size_t i = 0; i < count - 1; ++i) {
            if(values[i] > values[i+1]) {
                auto temp = values[i];
                values[i] = values[i+1];
                values[i+1] = temp;
            }
        }
    }
}

int main() {
    // create results file
    std::ofstream results("bubble.csv", std::ofstream::out);

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
            bubble_sort(data);
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