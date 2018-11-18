#include <condition_variable>
#include <iostream>
#include <chrono>
#include <mutex>
#include <thread>

std::mutex mut;

void task_1(std::condition_variable &condition) {
	std::cout << "Task 1 sleeping for 3 seconds" << std::endl;

	std::this_thread::sleep_for(std::chrono::seconds(3));
	auto lock = std::unique_lock<std::mutex>(mut);

	std::cout << "Task 1 notifying waiting thread" << std::endl;
	condition.notify_one();

	std::cout << "Task 1 waiting for notification" << std::endl;
	condition.wait(lock);

	std::cout << "Task 1 notified" << std::endl;

	std::cout << "Task 1 sleeping for 3 seconds" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(3));

	std::cout << "Task 1 notifying waiting thread" << std::endl;
	condition.notify_one();

	std::cout << "Task 1 waiting 3 seconds for notification" << std::endl;
	if (condition.wait_for(lock, std::chrono::seconds(3)) == std::cv_status::no_timeout)
		std::cout << "Task 1 notified before 3 seconds" << std::endl;
	else
		std::cout << "Task 1 got tired waiting" << std::endl;

	std::cout << "Task 1 finished" << std::endl;
}

void task_2(std::condition_variable &condition) {
	// Create lock
	auto lock = std::unique_lock<std::mutex>(mut);
	// Task two will initially wait for notification
	std::cout << "Task 2 waiting for notification" << std::endl;
	// Wait, releasing the lock as we do.
	condition.wait(lock);
	// We are free to continue
	std::cout << "Task 2 notified" << std::endl;
	// Sleep for 5 seconds
	std::cout << "Task 2 sleeping for 5 seconds" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(5));
	// Notify waiting thread
	std::cout << "Task 2 notifying waiting thread" << std::endl;
	condition.notify_one();
	// Now wait 5 seconds for notification
	std::cout << "Task 2 waiting 5 seconds for notification" << std::endl;
	if (condition.wait_for(lock, std::chrono::seconds(5)) == std::cv_status::no_timeout)
		std::cout << "Task 2 notified before 5 seconds" << std::endl;
	else
		std::cout << "Task 2 got tired waiting" << std::endl;
	// Sleep for 5 seconds
	std::cout << "Task 2 sleeping for 5 seconds" << std::endl;
	std::this_thread::sleep_for(std::chrono::seconds(5));
	// Notify any waiting thread
	std::cout << "Task 2 notifying waiting thread" << std::endl;
	condition.notify_one();
	// Print finished message
	std::cout << "Task 2 finished" << std::endl;
}

int main(int argc, char **argv)
{
	// Create condition variable
	std::condition_variable condition;

	// Create two threads
	std::thread t1(task_1, ref(condition));
	std::thread t2(task_2, ref(condition));

	// Join two threads
	t1.join();
	t2.join();

	getchar();
	return 0;
}