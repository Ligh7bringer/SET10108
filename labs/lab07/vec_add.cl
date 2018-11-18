__kernel void vecadd(__global int *A, __global int *B, __global int *C) { 
	// get the work item's unique ID
	int idx = get_global_id(0);
	// add vectors and store in C
	C[idx] = A[idx] + B[idx];
}
