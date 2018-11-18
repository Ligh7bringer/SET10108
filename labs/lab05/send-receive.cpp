#include <mpi.h>
#include <iostream>
#include <string>

using namespace std;

const unsigned int MAX_STRING = 100;

int main() {
	int num_procs, my_rank;

	//initialise MPI
	auto result = MPI_Init(nullptr, nullptr);
	if (result != MPI_SUCCESS) {
		cout << "ERROR - initialising MPI" << endl;
		MPI_Abort(MPI_COMM_WORLD, result);
		return -1;
	}

	// get MPI information
	MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

	// check if main process
	if (my_rank != 0) {
		// not main process - send message
		// generate message
		string data = "Greetings from process " + to_string(my_rank) + " of " + to_string(num_procs) + "!";
		MPI_Send((void*)data.c_str(), data.length() + 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
	} else {
		// main process - print message
		cout << "Greetings from process " << my_rank << " of " << num_procs << "!" << endl;
		// read in data from each worker process
		char message[MAX_STRING];
		for (int i = 1; i < num_procs; ++i) {
			// receive memory into buffer
			MPI_Recv(message, MAX_STRING, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			// Display message
			cout << message << endl;
		}
	}

	// shutdown MPI
	MPI_Finalize();

	return 0;
}