#define __CL_ENABLE_EXCEPTIONS

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <CL/cl.hpp>

using namespace std;
using namespace cl;

constexpr int ELEMENTS = 2048;
constexpr std::size_t DATA_SIZE = sizeof(int) * ELEMENTS;

int main(int argc, char** argv) {
	// init memory
	array<int, ELEMENTS> A;
	array<int, ELEMENTS> B;
	array<int, ELEMENTS> C;

	for (std::size_t i = 0; i < ELEMENTS; ++i) {
		A[i] = B[i] = i;
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

		// create a command q for device 0
		CommandQueue queue(context, devices[0]);

		// create buffers
		Buffer bufA(context, CL_MEM_READ_ONLY, DATA_SIZE);
		Buffer bufB(context, CL_MEM_READ_ONLY, DATA_SIZE);
		Buffer bufC(context, CL_MEM_WRITE_ONLY, DATA_SIZE);

		// copy data to the gpu
		queue.enqueueWriteBuffer(bufA, CL_TRUE, 0, DATA_SIZE, &A);
		queue.enqueueWriteBuffer(bufB, CL_TRUE, 0, DATA_SIZE, &B);

		// read kernel source
		ifstream file("vec_add.cl");
		string code(istreambuf_iterator<char>(file), (istreambuf_iterator<char>()));
		
		// create program
		Program::Sources source(1, make_pair(code.c_str(), code.length() + 1));
		Program program(context, source);

		// build program for devices
		program.build(devices);

		// create the kernel
		Kernel vecadd_kernel(program, "vecadd");

		// pass arguments
		vecadd_kernel.setArg(0, bufA);
		vecadd_kernel.setArg(1, bufB);
		vecadd_kernel.setArg(2, bufC);

		// execute kernel
		NDRange global(ELEMENTS);
		NDRange local(256);
		queue.enqueueNDRangeKernel(vecadd_kernel, NullRange, global, local);

		// copy result back
		queue.enqueueReadBuffer(bufC, CL_TRUE, 0, DATA_SIZE, &C);

		// Test that the results are correct
		for (int i = 0; i < 2048; ++i)
			if (C[i] != i + i)
				cout << "Error: " << i << endl;
			else
				cout << C[i] << endl;

		cout << "Finished" << endl;

	}
	catch (Error error) {
		cout << error.what() << "(" << error.err() << ")" << endl;
	}

	return 0;
}