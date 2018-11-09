#include <mpi.h>
#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

int main() {
	auto result = MPI_Init(nullptr, nullptr);
	if (result != MPI_SUCCESS) {
		MPI_Abort(MPI_COMM_WORLD, result);
		return -1;
	}

	int my_rank, num_procs;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	char send = 'a', receive;

	// perform 100 timings
	for (unsigned int i = 0; i < 100; ++i) {
		// sync with other processes
		MPI_Barrier(MPI_COMM_WORLD);

		// get start time
		auto start = system_clock::now();
		// send data back and forth
		if (my_rank == 0) {
			for (unsigned int j = 0; j < 100000; ++j) {
				// Ping
				MPI_Send(&send, 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
				// Pong
				MPI_Recv(&receive, 1, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
		else {
			for (unsigned int j = 0; j < 100000; ++j)
			{
				// Pong
				MPI_Recv(&receive, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				// Ping
				MPI_Send(&send, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
			}
		}
		// get time
		auto end = system_clock::now();
		auto total = duration_cast<nanoseconds>(end - start).count();
		// divide total by num of iterations to get time per iteration
		auto res = static_cast<double>(total) / 100000.0;
		// divide by 2 to get latency
		res /= 2.0;
		// show result
		if (my_rank == 0)
			cout << res << endl;
	}

	MPI_Finalize();

	return 0;
}