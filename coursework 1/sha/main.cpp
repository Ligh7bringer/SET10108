#include <iostream>
#include <string>
#include "block_chain.h"

using namespace std;

int main()
{
	cout << "# Format: block number, hash, time\n\n";

    for(size_t difficulty = 1; difficulty < 7; ++difficulty) {
        block_chain bchain(difficulty);
        for (uint32_t i = 1; i < 11u; ++i) {
            // cout << "Mining block " << i << "..." << endl;
            bchain.add_block(block(i, string("Block ") + to_string(i) + string(" Data")));
        }
    }

    return 0;
}