#include "threadsafe_stack.h"
#include <thread>
#include <iostream>

void pusher(const std::shared_ptr<threadsafe_stack<unsigned int>> &stack) {
    for(unsigned int i = 0; i < 1000000; ++i) {
        stack->push(i);
        std::this_thread::yield();
    }
}

void popper(const std::shared_ptr<threadsafe_stack<unsigned int>> &stack) {
    unsigned int count = 0;
    while(count < 1000000) {
        try {
            auto val = stack->pop();
            ++count;
        } catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
}

int main() {
    auto stack = std::make_shared<threadsafe_stack<unsigned int>>();

    std::thread t1(popper, stack);
    std::thread t2(pusher, stack);

    t1.join();
    t2.join();

    std::cout << "Stack empty = " << stack->empty() << std::endl;

    return 0;
}

