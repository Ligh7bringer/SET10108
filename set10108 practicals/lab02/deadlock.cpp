#include <thread>
#include <mutex>
#include <iostream>

std::mutex mut, mut2;

void task() {
    mut2.lock();
    std::cout << "Thread 1 running..." << std::endl;
    mut.lock();
    mut2.unlock();
    mut.unlock();
}

void task2() {
    mut.lock();
    std::cout << "Thread 2 running..." << std::endl;
    mut2.lock();
    mut.unlock();
    mut2.unlock();
}

int main() {
    std::thread t(task);
    std::thread t2(task2);

    t.join();
    t2.join();

    std::cout << "Done!" << std::endl;

    return 0;
}