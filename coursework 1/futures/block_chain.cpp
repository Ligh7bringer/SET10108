#include "block_chain.h"
#include "sha256.h"

#include <iostream>
#include <sstream>
#include <chrono>
#include <future>
#include <mutex>

using namespace std;
using namespace std::chrono;

// Note that _time would normally be set to the time of the block's creation.
// This is part of the audit a block chain.  To enable consistent results
// from parallelisation we will just use the index value, so time increments
// by one each time: 1, 2, 3, etc.
block::block(uint32_t index, const string &data)
: _index(index), _data(data), _nonce(0), _time(static_cast<long>(index)), _str(to_string(_index) + to_string(_time) + _data), _found(false)
{
}

void block::mine_block(uint32_t difficulty) noexcept
{
    string str(difficulty, '0');
	
	// get natively supported number of threads
    auto num_threads = thread::hardware_concurrency();

	// lambda function to run in each thread
    auto task = [&, this](uint32_t difficulty) {	
		auto target = string(difficulty, '0');
	
		// while the hash is not found
        while(!_found) {
			// generate a new hash
			auto ss = _str;
			// nonce is atomic so no need for mutexes
			// also append is faster!
			ss.append(to_string(++_nonce));
			ss.append(prev_hash);
			auto this_hash = sha256(ss);
			// chech if hash is valid
            if(this_hash.substr(0, difficulty) == target && !_found) {
				//stop this thread and notify other threads
                _found = true;		
				return this_hash;				            
            }
        }

        return string();
    };

    auto start = system_clock::now();

	// start asynchronous tasks
    vector<future<string>> futures;
    for(unsigned int i = 0; i < num_threads; ++i) {
		// futures are non-copyable so use move
        futures.push_back(move(async(task, difficulty)));
    }

	// check results
    for(auto &f : futures) {
        auto result = f.get();
		// if result is empty, ignore it
		if (!result.empty()) {
			// otherwise this is the hash we need
			_hash = result;
			break;
		}           
    }

    auto end = system_clock::now();
    duration<double> diff = end - start;

    cout << _hash << ", " << diff.count() << endl;
}

std::string block::calculate_hash() noexcept
{
    auto ss = _str;
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
    _chain.push_back(move(new_block));
}