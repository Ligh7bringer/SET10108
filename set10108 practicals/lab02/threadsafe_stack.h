#pragma once

#include <exception>
#include <stack>
#include <memory>
#include <mutex>

template<typename T>
class threadsafe_stack {
private:
	std::stack<T> data;
	mutable std::mutex mut;

public:
	threadsafe_stack() {}
	threadsafe_stack(const threadsafe_stack &other) {
		std::lock_guard<std::mutex> lock(other.mut);
		data = other.data;
	}

	void push(T value) {
		std::lock_guard<std::mutex> lock(mut);
		data.push(value);
	}

	T pop() {
		std::lock_guard<std::mutex> lock(mut);
		if (data.empty()) throw std::exception();
		auto res = data.top();
		data.pop();
		return res;
	}

    bool empty() const {
	    std::lock_guard<std::mutex> lock(mut);
	    return data.empty();
	}
};