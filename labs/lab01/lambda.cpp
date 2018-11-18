#include <iostream>
#include <functional>

int main() {
	//add 2 numbers
	auto add = [](int x, int y) { return x + y; };
	auto x = add(10, 12);
	std::cout << "10 + 12 = " << x << std::endl;

	//function objects
	std::function<int(int, int)> add_function = [](int x, int y) { return x + y; };
	x = add_function(20, 12);
	std::cout << "20 + 12 = " << x << std::endl;

	//pass fixed values
	int a = 5, b = 10;
	auto add_fixed = [a, b] { return a + b; };
	x = add_fixed();
	std::cout << "5 + 10 = " << x << std::endl;
	// Change the values of a and b
	a = 20; b = 30;
	// Call the fixed function again
	x = add_fixed();
	// Display the answer - will come out as 15
	std::cout << "20 + 30 = " << x << std::endl;
	
	//pass variables by reference
	auto add_reference = [&a, &b] { return a + b; };
	x = add_reference();
	std::cout << "20 + 30 = " << x << std::endl;
	a = 30; b = 5;
	x = add_reference();
	std::cout << "30 + 5 = " << x << std::endl;

	return 0;
}