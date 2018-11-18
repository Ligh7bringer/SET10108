#include <atomic>
#include <memory>
#include <thread>
#include <iostream>
#include <vector>

void increment(const std::shared_ptr<std::atomic<int>> &value) {
    for(unsigned int i = 0; i < 1000000; ++i)
        (*value)++;
}

int main() {
    auto value = std::make_shared<std::atomic<int>>();

    auto num_threads = std::thread::hardware_concurrency();
    std::cout << "Number of threads is " << num_threads << std::endl;
    std::vector<std::thread> threads;
    for(unsigned int i = 0; i < num_threads; ++i)
        threads.emplace_back(increment, value);

    for(auto &t : threads)
        t.join();

    std::cout << "Value = " << *value << std::endl;
}