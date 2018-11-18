#include <iostream>
#include <vector>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

using namespace std;

constexpr size_t ELEMENTS = 2048;

__global__ void vecadd(const int *A, const int *B, int *C) {
	// get block index
	unsigned int block_idx = blockIdx.x;
	// get thread index
	unsigned int thread_idx = threadIdx.x;
	// get number of threads per block
	unsigned int block_dim = blockDim.x;
	// get thread unique ID
	unsigned int idx = (block_idx * block_dim) + thread_idx;
	// add A and B and store in C
	C[idx] = A[idx] + B[idx];
}

int main(int argc, char** argv) {
	// create host memory
	auto data_size = sizeof(int) * ELEMENTS;
	vector<int> A(ELEMENTS);    // input aray
	vector<int> B(ELEMENTS);    // input array
	vector<int> C(ELEMENTS);    // output array

	// initialise input data
	for (unsigned int i = 0; i < ELEMENTS; ++i)
		A[i] = B[i] = i;

	// declare buffers
	int *buffer_A, *buffer_B, *buffer_C;

	// init buffers
	cudaMalloc((void**)&buffer_A, data_size);
	cudaMalloc((void**)&buffer_B, data_size);
	cudaMalloc((void**)&buffer_C, data_size);
	
	// write host data to device
	cudaMemcpy(buffer_A, &A[0], data_size, cudaMemcpyHostToDevice);
	cudaMemcpy(buffer_B, &B[0], data_size, cudaMemcpyHostToDevice);

	// run kernel with one thread for each element
	vecadd<<<ELEMENTS / 1024, 1024>>>(buffer_A, buffer_B, buffer_C);

	// wait for kernel to complete
	cudaDeviceSynchronize();

	// read output buffer back to host
	cudaMemcpy(&C[0], buffer_C, data_size, cudaMemcpyDeviceToHost);

	// clean up
	cudaFree(buffer_A);
	cudaFree(buffer_B);
	cudaFree(buffer_C);

	// Test that the results are correct
	for (int i = 0; i < 2048; ++i)
		if (C[i] != i + i) {
			cout << "Error: " << i << endl;
			break;
		} else
			cout << C[i] << endl;

	cout << "Finished" << endl;

	return 0;
}