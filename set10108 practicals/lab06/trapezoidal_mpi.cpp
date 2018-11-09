#include <omp.h>
#include <iostream>
#include <functional>
#include <memory>
#include <cmath>
#include <thread>
#include <mpi.h>

double trap(std::function<double(double)> f, double start, double end, size_t iterations, int my_rank, int num_procs) {
	// calculate iteration slice size
	auto slice_size = (end - start) / iterations;
	// calculate num of iterations per thread
	auto iterations_thread = iterations / num_procs;
	// calculate this thread's start point
	auto local_start = start + ((my_rank * iterations_thread) * slice_size);
	// calculate this thread's end point
	auto local_end = local_start + iterations_thread * slice_size;
	// calculate initial result
	auto my_result = (f(local_start) + f(local_end)) / 2.0;

	// make sure x is preserved during iterations by predeclaring
	double x;
	// sum each iteration
	for (size_t i = 0; i <= iterations_thread - 1; ++i) {
		// calculate next slice to calculate
		x = local_start + i * slice_size;
		// add to current result
		my_result += f(x);
	}
	// multiply the result by the slice size
	my_result *= slice_size;

	// add to shared data
	return my_result;
}

int main() {
	// initialise MPI
	auto result = MPI_Init(nullptr, nullptr);
	if (result != MPI_SUCCESS) {
		MPI_Abort(MPI_COMM_WORLD, result);
		return -1;
	}

	int my_rank, num_procs;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	// define start and end values
	auto start = 0.0;
	auto end = 3.14159265359; // pi

	// Sync
	MPI_Barrier(MPI_COMM_WORLD);

	// function to calculate integral of; cos
	auto f = [](double x) { return cos(x); };

	// perform calculation
	unsigned int iterations = static_cast<unsigned int>(pow(2, 24));
	auto local_result = trap(f, start, end, iterations, my_rank, num_procs);

	// reduce result
	double global_result = 0.0;
	MPI_Reduce(&local_result, &global_result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	
	// If main process, display result
	if (my_rank == 0)
		std::cout << "Area under curve: " << global_result << std::endl;

	MPI_Finalize();

	return 0;
}