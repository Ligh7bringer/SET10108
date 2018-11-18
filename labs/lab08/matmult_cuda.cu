#include <iostream>
#include <vector>
#include <cuda_runtime.h>
#include <device_launch_parameters.h>

using namespace std;

constexpr unsigned int ELEMENTS = 32;

// cuda kernel
__global__ void simple_multiply(float *output_C, unsigned int width_A, unsigned int height_A, unsigned int width_B, unsigned int height_B, const float *input_A, const float *input_B)
{
	// Get global position in Y direction
	unsigned int row = (blockIdx.y * 1024) + threadIdx.y;
	// Get global position in X direction
	unsigned int col = (blockIdx.x * 1024) + threadIdx.x;

	float sum = 0.0f;

	// Calculate result of one element of matrix C
	for (unsigned int i = 0; i < width_A; ++i)
		sum += input_A[row * width_A + i] * input_B[i * width_B + col];

	// Store result in matrix C
	output_C[row * width_B + col] = sum;
}

int main(int argc, char** argv) {
	// create host memory
	auto data_size = sizeof(float) * ELEMENTS * ELEMENTS;
	vector<vector<float>> A(ELEMENTS, vector<float>(ELEMENTS, 1));    // input aray, fill with ones
	vector<vector<float>> B(ELEMENTS, vector<float>(ELEMENTS, 1));    // input array, fill with ones
	vector<vector<float>> C(ELEMENTS, vector<float>(ELEMENTS));    // output array, fill with zeroes

	// create buffers
	float *buf_A, *buf_B, *buf_C;

	// init buffers
	cudaMalloc((void**)&buf_A, data_size);
	cudaMalloc((void**)&buf_B, data_size);
	cudaMalloc((void**)&buf_C, data_size);

	// send data to gpu
	cudaMemcpy(buf_A, &A[0], data_size, cudaMemcpyHostToDevice);
	cudaMemcpy(buf_B, &B[0], data_size, cudaMemcpyHostToDevice);

	dim3 blocksPerGrid(512, 512);
	dim3 threadsPerBlock(32, 32);	
	// run kernel
	simple_multiply<<<blocksPerGrid, threadsPerBlock>>>(buf_C, ELEMENTS, ELEMENTS, ELEMENTS, ELEMENTS, buf_A, buf_B);

	// wait for kernel to complete
	cudaDeviceSynchronize();

	// check for errors
	cudaError_t error = cudaGetLastError();
	if (error != cudaSuccess)
	{
		fprintf(stderr, "ERROR: %s\n", cudaGetErrorString(error));
		return -1;
	}

	// read output buffer back to host
	cudaMemcpy(&C[0], buf_C, data_size, cudaMemcpyDeviceToHost);
	   
	// clean up
	cudaFree(buf_A);
	cudaFree(buf_B);
	cudaFree(buf_C);

	// check results
	for (unsigned int row = 0; row < ELEMENTS; ++row) {
		for (unsigned int col = 0; col < ELEMENTS; ++col) {
			//cout << C[row][col] << ", ";
		}
		cout << endl;
	}

	return 0;
}