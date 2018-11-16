#define __CL_ENABLE_EXCEPTIONS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <CL/cl.hpp>

using namespace std;
using namespace cl;

constexpr unsigned int ELEMENTS = 64;
constexpr std::size_t DATA_SIZE = sizeof(float) * ELEMENTS * ELEMENTS;

int main(int argc, char** argv) {
	// initialise matrices
	array<array<float, ELEMENTS>, ELEMENTS> matA;
	array<array<float, ELEMENTS>, ELEMENTS> matB;
	array<array<float, ELEMENTS>, ELEMENTS> matC;

	for (unsigned int i = 0; i < ELEMENTS; ++i) {
		for (unsigned int j = 0; j < ELEMENTS; ++j) {
			matA[i][j] = matB[i][j] = 1;
		}
	}

	try {
		// get platforms
		vector<Platform> platforms;
		Platform::get(&platforms);

		// get gpu devices
		vector<Device> devices;
		platforms[0].getDevices(CL_DEVICE_TYPE_GPU, &devices);
		// debug
		cout << devices[0].getInfo<CL_DEVICE_NAME>() << endl;

		// create a context
		Context context(devices);

		// create a command queue
		CommandQueue queue(context, devices[0]);

		// create buffers
		Buffer bufA(context, CL_MEM_READ_ONLY, DATA_SIZE);
		Buffer bufB(context, CL_MEM_READ_ONLY, DATA_SIZE);
		Buffer bufC(context, CL_MEM_WRITE_ONLY, DATA_SIZE);

		// copy data to gpu
		queue.enqueueWriteBuffer(bufA, CL_TRUE, 0, DATA_SIZE, &matA);
		queue.enqueueWriteBuffer(bufB, CL_TRUE, 0, DATA_SIZE, &matB);

		// read kernel source
		ifstream file("mat_mult.cl");
		string code(istreambuf_iterator<char>(file), (istreambuf_iterator<char>()));

		// create program
		Program::Sources source(1, make_pair(code.c_str(), code.length() + 1));
		Program program(context, source);

		// build program for devices
		program.build(devices);

		// create kernel
		Kernel mult_kernel(program, "simple_multiply");

		mult_kernel.setArg(0, bufC);
		mult_kernel.setArg(1, ELEMENTS);
		mult_kernel.setArg(2, ELEMENTS);
		mult_kernel.setArg(3, ELEMENTS);
		mult_kernel.setArg(4, ELEMENTS);
		mult_kernel.setArg(5, bufA);
		mult_kernel.setArg(6, bufB);

		// execute kernel
		// since we are using 2 dimensions, global and local size should be 2-dimensional too
		NDRange global(ELEMENTS, ELEMENTS);
		NDRange local(ELEMENTS / 2, ELEMENTS / 2);
		queue.enqueueNDRangeKernel(mult_kernel, NullRange, global, local);

		// copy result back
		queue.enqueueReadBuffer(bufC, CL_TRUE, 0, DATA_SIZE, &matC);

		// check results
		for (unsigned int row = 0; row < ELEMENTS; ++row) {
			for (unsigned int col = 0; col < ELEMENTS; ++col) {
				cout << matC[row][col] << ", ";
			}
			cout << endl;
		}

	}
	catch (Error error) {
		cout << error.what() << ": " << error.err() << endl;
	}

	return 0;
}