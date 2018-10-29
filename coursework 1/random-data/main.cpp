#include <iostream>
#include <string>
#include <algorithm>

#include "block_chain.h"

using namespace std;

/*
* This program was used to test whether the length of block data has any effect on speed.
*/

// generates random string with given length
// thanks to https://stackoverflow.com/a/12468109
std::string random_string(size_t length)
{
	auto randchar = []() -> char
	{
		// charactes to be used in the creation of random string
		const char charset[] =
			"0123456789"
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz";
			"!£$%^&*()-=+`#~";
		// max index in the array
		const size_t max_index = (sizeof(charset) - 1);
		// return random character
		return charset[rand() % max_index];
	};

	// generate string of given length
	string str(length, 0);
	generate_n(str.begin(), length, randchar);
	return str;
}

int main()
{
	// run 10 times with difficulty set to three
	for(size_t j = 1; j < 11; ++j) {
        block_chain bchain(3);	
		auto len = j * 10; // set length of data
		auto data = random_string(len); // generate random data with this length
		cout << "# data length, " << len << ", " << data << endl;
        for (uint32_t i = 1; i < 11u; ++i) {		
			//cout << data << endl;
            bchain.add_block(block(i, data)); // use the random data
        }
    }

    return 0;
}