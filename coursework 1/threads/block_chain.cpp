#include "block_chain.h"
#include "sha256.h"

#include <sstream>
#include <chrono>
#include <mutex>
#include <thread>
#include <fstream>

using namespace std;
using namespace std::chrono;

mutex mut;

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
    // number of threads natively supported
    auto num_threads = thread::hardware_concurrency();
    // vector to store the threads
    vector<thread> threads;
	// shared flag used to stop threads 
    auto flag = make_shared<atomic<bool>>(false);

    // lambda function each thread is going to run
    auto task = [&](uint32_t difficulty, shared_ptr<atomic<bool>> flag) {
		// number of leading zeroes we are looking for
        string target(difficulty, '0');
        // while loop which is going to run until one of the threads calculates the correct hash
        while(!(*flag)) {
            // calculate a hash
			// nonce is incremented in calculate_hash()
			// no need to protect it as it is atomic
            auto this_hash = calculate_hash();

            // if this is the hash we are looking for
            if (this_hash.substr(0, difficulty) == target && !(*flag)) {
                // set flag to true so other threads know they should stop
                *flag = true;
                // set _hash
                _hash = this_hash;
            }
        }
    };

    // get start time
    auto start = system_clock::now();

    // create threads
    for(size_t i = 0; i < num_threads; ++i)
        threads.emplace_back(task, difficulty, flag);

    // wait for threads to finish
    for(auto &t : threads)
        t.join();

    // get end time
    auto end = system_clock::now();
    // calculate run time
    duration<double> diff = end - start;

    cout << _hash << ", " << diff.count() << endl;
}

std::string block::calculate_hash() noexcept
{
	// append is faster!
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
	// use move constructor
    _chain.push_back(std::move(new_block));
}