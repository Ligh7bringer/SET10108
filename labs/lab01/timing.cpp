#include <thread>
#include <chrono>
#include <fstream>
#include <iostream>

void work() {
	int n = 0;
	for (int i = 0; i < 1000000; ++i) 
		++n;
}

int main() {
	std::ofstream data("data.csv", std::ofstream::out);

	for (int i = 0; i < 100; ++i) {
		auto start = std::chrono::system_clock::now();

		std::thread t(work);
		t.join();

		auto end = std::chrono::system_clock::now();
		auto total = end - start;
		data << total.count() << std::endl;
	}

	data.close();

	getchar();
	return 0;
}
