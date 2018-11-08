#include <mpi.h>
#include <vector>
#include <random>
#include <chrono>
#include <iostream>

using namespace std;
using namespace chrono;

constexpr size_t SIZE = 1 << 20;

// Randomly generate vector values
void generate_data(vector<float> &data) {
	// create random engine
	auto millis = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
	default_random_engine e(static_cast<unsigned int>(millis.count()));
	// fill data
	for (unsigned int i = 0; i < data.size(); ++i)
		data[i] = e();
}

void normalise_vector(vector<float> &data)
{
	// Iterate through each 4-dimensional vector
	for (unsigned int i = 0; i < (data.size() / 4); ++i)
	{
		// sum the squares of the 4 components
		float sum = 0.0f;
		for (unsigned int j = 0; j < 4; ++j) {
			sum += powf(data[(i * 4) + j], 2.0f);
		}

		//get the sqrt of of the result
		sum = sqrtf(sum);
		// divide each component by sum
		for (unsigned int j = 0; j < 4; ++j) {
			data[(i * 4) + j] /= sum;
		}
	}
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

	vector<float> data;
	vector<float> my_data(SIZE / num_procs);

	// if main process
	if (my_rank == 0) {
		// gen data
		data.resize(SIZE);
		generate_data(data);
		// scatter the data
		MPI_Scatter(&data[0], SIZE / num_procs, MPI_FLOAT,  // source
			&my_data[0], SIZE / num_procs, MPI_FLOAT, // destination
			0, MPI_COMM_WORLD);
	} else {
		MPI_Scatter(nullptr, SIZE / num_procs, MPI_FLOAT, // source
			&my_data[0], SIZE / num_procs, MPI_FLOAT,  //dest
			0, MPI_COMM_WORLD);
	}

	// normalise local data
	normalise_vector(data);

	// gather results
	if (my_rank == 0) {
		MPI_Gather(&data[0], SIZE / num_procs, MPI_FLOAT,  // source
			&my_data[0], SIZE / num_procs, MPI_FLOAT, // destination
			0, MPI_COMM_WORLD);
	}
	else {
		MPI_Gather(&my_data[0], SIZE / num_procs, MPI_FLOAT,// Source
			nullptr, SIZE / num_procs, MPI_FLOAT,           // Dest
			0, MPI_COMM_WORLD);
	}

	// Check if main process
	if (my_rank == 0)
	{
		// Display results - first 10
		for (unsigned int i = 0; i < 10; ++i)
		{
			cout << "<";
			for (unsigned int j = 0; j < 3; ++j)
				cout << data[(i * 4) + j] << ", ";
			cout << data[(i * 4) + 3] << ">" << endl;
		}
	}

	MPI_Finalize();

	return 0;
}