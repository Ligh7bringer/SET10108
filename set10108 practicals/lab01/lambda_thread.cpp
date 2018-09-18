#include <thread>
#include <iostream>

int main() {
	std::thread t([] { std::cout << "Hello from lambda thread!" << std::endl; });
	t.join();

	getchar();
	return EXIT_SUCCESS;
}