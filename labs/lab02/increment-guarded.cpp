#include <mutex>
#include <memory>
#include <vector>
#include <iostream>
#include <thread>

std::mutex mut;

void increment(std::shared_ptr<int> value) {
	for (unsigned int i = 0; i < 1000000; ++i) {
		std::lock_guard<std::mutex> lock(mut);
		*value = *value + 1;
	}
}

int main() {
	auto value = std::make_shared<int>(0);

	auto num_threads = std::thread::hardware_concurrency();
	std::cout << "Number of threads = " << num_threads << std::endl;
	std::vector<std::thread> threads;

	for (unsigned int i = 0; i < num_threads; ++i)
		threads.emplace_back(increment, value);

	for (auto &t : threads)
		t.join();

	std::cout << "Value = " << *value << std::endl;

	getchar();
	return 0;
}