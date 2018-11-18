#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <future>
#include <string>
#include <mpi.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "../../lib/stb_image_write.h"

// max iterations to perform to find pixel value
constexpr size_t max_iterations = 1000;

// mandelbrot dimensions ([-2.1, 1.0], [-1.3, 1.3])
constexpr double xmin = -2.1;
constexpr double xmax = 1.0;
constexpr double ymin = -1.3;
constexpr double ymax = 1.3;

std::vector<double> mandelbrot(unsigned int dim, size_t start_y, size_t end_y) {
	// conversion from mandelbrot coordinate to image coordinate
	auto integral_x = (xmax - xmin) / static_cast<double>(dim);
	auto integral_y = (ymax - ymin) / static_cast<double>(dim);

	// declare needed variables
	double x, y, x1, y1, xx = 0.0;
	size_t loop_count = 0;
	// vector to store the results
	std::vector<double> results;

	// loop through each line
	y = ymin + (start_y * integral_y);
	for (size_t y_coord = start_y; y_coord < end_y; ++y_coord) {
		x = xmin;
		// loop through each pixel on this line
		for (size_t x_coord = 0; x_coord < dim; ++x_coord) {
			x1 = 0.0, y1 = 0.0;
			loop_count = 0;
			// calculate mandelbrot value
			while (loop_count < max_iterations && std::sqrt((x1 * x1) + (y1 * y1)) < 2.0) {
				++loop_count;
				xx = (x1 * x1) - (y1 * y1) + x;
				y1 = 2 * x1 * y1 + y;
				x1 = xx;
			}

			// get number of iterations of the loop
			auto val = static_cast<double>(loop_count) / static_cast<double>(max_iterations);
			// push into the vector
			results.push_back(val * 255);
			// increase x based on the integral
			x += integral_x;
		}

		// increase y based on integral
		y += integral_y;
	}

	return results;
}

int main() {
	// initialise MPI
	auto result = MPI_Init(nullptr, nullptr);
	if (result != MPI_SUCCESS) {
		MPI_Abort(MPI_COMM_WORLD, result);
		return -1;
	}

	// get rank of process and number of processes
	int my_rank, num_procs;
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

	// dimensions in pixels of image to generate
	unsigned int dim = 0;
	if (my_rank == 0) {
		dim = 8192;
		// broadcast dimensions to all workers
		MPI_Bcast(&dim, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
	}
	else {
		// get dimension
		MPI_Bcast(&dim, 1, MPI_UNSIGNED, 0, MPI_COMM_WORLD);
	}

	// determine strip height
	size_t strip_height = dim / num_procs;

	// perform mandelbrot
	std::cout << "Calculating...." << std::endl;
	auto res = mandelbrot(dim, my_rank * strip_height, (my_rank + 1) * strip_height);

	// vector to store results in
	std::vector<float> data(res.size() * num_procs);
	// check if main process
	if (my_rank == 0) {
		// resize data to gather results
		data.resize(res.size() * num_procs);
	}

	// gather results
	std::cout << "Gathering results..." << std::endl;
	MPI_Gather(&res[0], res.size(), MPI_FLOAT, &data[0], res.size(), MPI_FLOAT, 0, MPI_COMM_WORLD);

	MPI_Finalize();

	// create image from the generated data	
	if (my_rank == 0) {
		auto filename = std::to_string(my_rank) + "fractal.png";
		std::cout << "Writing image " << filename << "..." << std::endl;
		if (!stbi_write_png(filename.c_str(), dim, dim, 4, &data[0], sizeof(float) * dim)) {
			std::cout << "Error writing image " << filename << std::endl;
			return -10;
		}

		std::cout << "Done!" << std::endl;
	}


	return 0;
}