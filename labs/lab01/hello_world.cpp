// set10108 practicals.cpp : Defines the entry point for the application.
//

#include "hello_world.h"

#include <thread>

void hello_world() {
	std::cout << "Hello world from thread " << std::this_thread::get_id() << std::endl;
}

int main()
{
	std::thread t(hello_world);

	t.join();

	getchar();
	return 0;
}
