#define __CL_ENABLE_EXCEPTIONS

#include <iostream>
#include <vector>
#include <CL/cl.hpp>

using namespace std;
using namespace cl;

int main(int argc, char **argv)
{
	try
	{
		// Get the platforms
		vector<Platform> platforms;
		Platform::get(&platforms);

		// Assume only one platform.  Get GPU devices.
		vector<Device> devices;
		platforms[0].getDevices(CL_DEVICE_TYPE_ALL, &devices);

		// priint all devices
		for(const auto& d : devices)
			cout << d.getInfo<CL_DEVICE_NAME>() << endl;

		// Create a context with these devices
		Context context(devices);

		// Create a command queue for device 0
		CommandQueue queue(context, devices[0]);
	}
	catch (Error error)
	{
		cout << error.what() << "(" << error.err() << ")" << endl;
	}
	return 0;
}