#include <atomic>
#include <memory>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

void task(unsigned int id, const std::shared_ptr<std::atomic_flag> &flag) {
    for(unsigned int i = 0; i < 10; ++i) {
        //check if flag is available. Program will be stuck in the while loop
        //until it becomes available
        while(flag->test_and_set());
        //flag is available
        std::cout << "Thread " << id << " running " << i << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
        //clear flag
        flag->clear();
    }
}

int main() {
    //create shared flag
    auto flag = std::make_shared<std::atomic_flag>();

    auto num_threads = std::thread::hardware_concurrency();
    std::cout << "Using " << num_threads << " threads." << std::endl;

    std::vector<std::thread> threads;
    for(unsigned int i = 0; i < num_threads; ++i)
        threads.emplace_back(task, i, flag);

    for(auto &t : threads)
        t.join();

    return 0;
}