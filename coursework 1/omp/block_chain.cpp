#include "block_chain.h"
#include "sha256.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <atomic>
#include <thread>
#include <omp.h>
#include <mutex>

using namespace std;
using namespace std::chrono;

// Note that _time would normally be set to the time of the block's creation.
// This is part of the audit a block chain.  To enable consistent results
// from parallelisation we will just use the index value, so time increments
// by one each time: 1, 2, 3, etc.
block::block(uint32_t index, const string &data)
: _index(index), _data(data), _nonce(0), _time(static_cast<long>(index)), _str(to_string(_index) + to_string(_time) + _data)
{
}

void block::mine_block(uint32_t difficulty) noexcept
{
	string str(difficulty, '0');

	// get number of threads natively supported
	auto thread_count = thread::hardware_concurrency() - 1;

	// shared boolean to stop the threads when hash is found
	std::atomic_bool found(false);

	auto start = system_clock::now();		
	string this_hash;

	// run in parallel with 8 threads, each thread gets a copy of this_hash
	#pragma omp parallel num_threads(thread_count) default(shared) private(this_hash)
	while (!found) {	
		// calculate hash, nonce is atomic so no need for protection
		this_hash = calculate_hash();

		// if this is the correct hash
		if (this_hash.substr(0, difficulty) == str && !found) {
			// set flag to true so threads know they can stop
			found = true;
			// protect _hash just in case another thread tries to set it
			#pragma omp critical
			_hash = this_hash;
		}	
	}

	auto end = system_clock::now();
	duration<double> diff = end - start;

	cout << _hash << ", " << diff.count() << endl;
}

std::string block::calculate_hash() noexcept
{
	// .append is faster than stringstream
    string ss = _str;
	ss.append(to_string(++_nonce));
	ss.append(prev_hash);
    return sha256(ss);
}

block_chain::block_chain(uint32_t difficulty)
{
    _chain.emplace_back(block(0, "Genesis Block"));
    _difficulty = difficulty;
	cout << "# difficulty = " << _difficulty << endl;
}

void block_chain::add_block(block &&new_block) noexcept
{
    new_block.prev_hash = get_last_block().get_hash();
    new_block.mine_block(_difficulty);
	// move constructor can be used 
    _chain.push_back(std::move(new_block));
}