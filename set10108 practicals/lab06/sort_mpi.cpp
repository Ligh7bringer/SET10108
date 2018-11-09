#include <vector>
#include <iostream>
#include<mpi.h>
#include <algorithm>
#include <random>

constexpr size_t SIZE = 1 << 10; // 2 ^ 10

std::vector<unsigned int> generate_values(size_t size) {
	// create random engine
	std::random_device r;
	std::default_random_engine e(r());
	// generate numbers
	std::vector<unsigned int> data;
	for (size_t i = 0; i < size; ++i)
		data.push_back(e());

	return data;
}

// merges the largest n values in local_data and temp_b into temp_c
// and copies temp_c into local data
void merge_high(std::vector<unsigned int>& local_data, std::vector<unsigned int>& temp_b, std::vector<unsigned int>& temp_c) {
	int ai, bi, ci;
	// get starting size
	ai = bi = ci = local_data.size() - 1;
	// loop through each value and store highest in temp_c
	for (; ci >= 0; --ci) {
		// find largest from local_data and temp_b
		if (local_data[ai] >= temp_b[bi]) {
			temp_c[ci] = local_data[ai];
			--ai;
		}
		else {
			temp_c[ci] = temp_b[bi];
			--bi;
		}
	}
	// copy temp_c into local data
	std::copy(temp_c.begin(), temp_c.end(), local_data.begin());
}

// merges the smallest n values in local_data and temp_b into temp_c
// and copies temp_c into local data
void merge_low(std::vector<unsigned int>& local_data, std::vector<unsigned int>& temp_b, std::vector<unsigned int>& temp_c) {
	int ai, bi, ci;
	// start at 0
	ai = bi = ci = 0;
	// store relevant smallest value in temp_c
	for (; ci < local_data.size(); ++ci) {
		// find smallest from local data and temp_b
		if (local_data[ai] <= temp_b[bi]) {
			temp_c[ci] = local_data[ai];
			++ai;
		}
		else {
			temp_c[ci] = temp_b[bi];
			++bi;
		}
	}
	// copy temp c into local data
	std::copy(temp_c.begin(), temp_c.end(), local_data.begin());
}

void odd_even_iter(std::vector<unsigned int>& local_data, std::vector<unsigned int>& temp_b, 
	std::vector<unsigned int>& temp_c, unsigned int phase, int even_partner, int odd_partner, unsigned int my_rank, unsigned int num_procs) {
	// operate based on phase
	if (phase % 2 == 0) {
		// check if even partner is valid
		if (even_partner >= 0) {
			// exchange data with even partner
			MPI_Sendrecv(&local_data[0], local_data.size(), MPI_UNSIGNED, even_partner, 0,
				&temp_b[0], temp_b.size(), MPI_UNSIGNED, even_partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// merge results
			if (my_rank % 2 == 0) 
				merge_low(local_data, temp_b, temp_c);
			else
				merge_high(local_data, temp_b, temp_c);
		}
	} 
	else {
		// check if odd partner is valid
		if (odd_partner >= 0) {
			// exchange data with odd partner
			MPI_Sendrecv(&local_data[0], local_data.size(), MPI_UNSIGNED, odd_partner, 0,
				&temp_b[0], temp_b.size(), MPI_UNSIGNED, odd_partner, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// merge results accordingly
			if (my_rank % 2 == 0)
				merge_high(local_data, temp_b, temp_c);
			else
				merge_low(local_data, temp_b, temp_c);
		}
	}
}

void odd_even_sort(std::vector<unsigned int> &local_data, unsigned int my_rank, unsigned int num_procs) {
	// temporary storage
	std::vector<unsigned int> temp_b(local_data);
	std::vector<unsigned int> temp_c(local_data);
	
	int even_partner, odd_partner;
	// find partners
	if (my_rank % 2 == 0) {
		even_partner = static_cast<int>(my_rank) + 1;
		odd_partner = static_cast<int>(my_rank) - 1;
		// check if partner is valid
		if (even_partner == num_procs)
			even_partner = MPI_PROC_NULL;
	}
	else {
		even_partner = static_cast<int>(my_rank) - 1;
		odd_partner = static_cast<int>(my_rank) + 1;
		// check if partner is valid
		if (odd_partner == num_procs)
			odd_partner = MPI_PROC_NULL;
	}

	std::sort(local_data.begin(), local_data.end());

	// Phased odd-even transposition sort
	for (unsigned int phase = 0; phase < num_procs; ++phase)
		odd_even_iter(local_data, temp_b, temp_c, phase, even_partner, odd_partner, my_rank, num_procs);
}

int main() {
	auto result = MPI_Init(nullptr, nullptr);
	if (result != MPI_SUCCESS) {
		MPI_Abort(MPI_COMM_WORLD, result);
		return -1;
	}

	int my_rank, num_procs;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	// data to sort
	std::vector<unsigned int> data(SIZE);
	if(my_rank == 0)
		data = generate_values(SIZE);

	// allocate enough space for local data
	std::vector<unsigned int> local_data(SIZE / num_procs);

	// scatter data
	std::cout << my_rank << ": Scattering\n";
	MPI_Scatter(&data[0], SIZE / num_procs, MPI_UNSIGNED, &local_data[0], SIZE / num_procs, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

	// Sort the data
	std::cout << my_rank << ": Sorting\n";
	odd_even_sort(local_data, my_rank, num_procs);

	// gather results
	std::cout << my_rank << ": Gathering\n";
	MPI_Gather(&local_data[0], SIZE / num_procs, MPI_UNSIGNED, &data[0], SIZE / num_procs, MPI_UNSIGNED, 0, MPI_COMM_WORLD);

	// print sorted vector
	if (my_rank == 0) {
		for (unsigned int i = 0; i < data.size(); ++i)
			std::cout << data[i] << std::endl;
	}

	MPI_Finalize();
	return 0;
}