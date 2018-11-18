#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include "guarded.h"

constexpr unsigned int NUM_ITERATIONS = 1000000;
constexpr unsigned int NUM_THREADS = 4;

void task(std::shared_ptr<guarded> g) {
	for (unsigned int i = 0; i < NUM_ITERATIONS; ++i)
		g->increment();
}

int main() {
	auto g = std::make_shared<guarded>();

	std::vector<std::thread> threads;
	for (unsigned int i = 0; i < NUM_THREADS; ++i)
		threads.emplace_back(task, g);

	for (auto &t : threads)
		t.join();

	std::cout << "Value = " << g->get_value() << std::endl;

	getchar();
	return 0;
}