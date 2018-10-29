#pragma once

#include <string>
#include <vector>
#include <atomic>
#include <memory>
#include <iostream>

class block
{
private:
    // The index of the block in the chain.
    uint32_t _index;
    // A modifier used to get a suitable block.
	// atomic to avoid race conditions
    std::atomic<uint64_t> _nonce;
    // Data stored in the block.
    std::string _data;
    // Hash code of this block.
    std::string _hash;
    // Time code block was created.
    long _time;
    std::string _str;

    std::string calculate_hash() noexcept;

public:
    block(uint32_t index, const std::string &data);

	// copy constructor is not needed as atomic member variables are non-copyable
    block(const block& other) = delete;

	// move constructor
    block(block&& other) noexcept : _nonce(other._nonce.load()), _data(std::move(other._data)), _index(other._index),
                                   _time(other._time), prev_hash(std::move(other.prev_hash)),
                                   _str(std::move(other._str)), _hash(std::move(other._hash)) {}

    // Difficulty is the minimum number of zeros we require at the
    // start of the hash.
    void mine_block(uint32_t difficulty) noexcept;

    inline const std::string& get_hash() const noexcept { return _hash; }

    // Hash code of the previous block in the chain.
    std::string prev_hash;
};

class block_chain
{
private:
    uint32_t _difficulty;
    std::vector<block> _chain;

    inline const block& get_last_block() const noexcept { return _chain.back(); }

public:
	block_chain(uint32_t difficulty);

    void add_block(block &&new_block) noexcept;
};