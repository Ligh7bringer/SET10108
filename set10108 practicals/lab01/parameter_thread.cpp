#include <thread>
#include <iostream>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

constexpr size_t num_threads = 100;

void task(size_t n, int val)
{
	std::cout << "Thread: " << n << " Random Value: " << val << std::endl;	
}

int main() {
	std::random_device r;

	default_random_engine e(r());

	vector<thread> threads;
	for (size_t i = 0; i < num_threads; ++i) {
		threads.push_back(thread(task, i, e()));
	}

	for (auto &t : threads) {
		t.join();
	}

	getchar();
	return EXIT_SUCCESS;
}