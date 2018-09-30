#include <future>
#include <vector>
#include <thread>
#include <iostream>
#include <chrono>
#include <random>

unsigned int find_max(const std::vector<unsigned int> &data, size_t start, size_t end) {
    unsigned int max = 0;
    for(unsigned int i = start; i < end; ++i)
        if(data.at(i) > max) {
            max = data.at(i);
            std::cout << "New max found: " << max << std::endl;
        }

    return max;
}

int main() {
    // get number of supported threads
    auto num_threads = std::thread::hardware_concurrency();
    std::cout << "Supported threads: " << num_threads << std::endl;
    std::cout << "Using " << num_threads - 1 << " futures." << std::endl;

    // Create a vector with 2^24 random values
    std::vector<unsigned int> values;
    auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    std::default_random_engine e(static_cast<unsigned int>(millis.count()));
    for(unsigned int i = 0; i < pow(2, 24); ++i)
        values.push_back(e());

    // create num_threads - 1 futures
    std::vector<std::future<unsigned int>> futures;
    auto range = static_cast<size_t>(pow(2, 24) / num_threads);
    for(size_t i = 0; i < num_threads - 1; ++i)
        futures.push_back(std::async(find_max, std::ref(values), i * range, (i + 1) * range));

    // main thread will process the end of the list
    auto max = find_max(values, (num_threads - 1) * range, num_threads * range);

    // get results from futures
    for(auto &f : futures) {
        auto result = f.get();
        if(result > max)
            max = result;
    }

    std::cout << "-------------------------------" << std::endl;
    std::cout << "Maximum value found: " << max << std::endl;

    return 0;
}