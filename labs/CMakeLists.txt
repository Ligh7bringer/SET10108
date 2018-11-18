# CMakeList.txt : CMake project for set10108 practicals, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.11)

# Use OpenMP
find_package(OpenMP)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

find_package(OpenCL)
include_directories(${OpenCL_INCLUDE_DIRS})
link_libraries(${OpenCL_LIBRARIES})

# Use MPI
find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})

# Use CUDA
# enable_language doesn't seem to work?
find_package(CUDA 9.0 REQUIRED)

# Use SIMD
# set(CMAKE_CXX_FLAGS "-msse4 -mavx")

# Add source to this project's executable.
# lab 1
add_executable (hello_world "lab01/hello_world.cpp" "lab01/hello_world.h")
add_executable (multiple_tasks "lab01/multiple_tasks.cpp")
add_executable (parameter_thread "lab01/parameter_thread.cpp")
add_executable (lambda "lab01/lambda.cpp")
add_executable (lambda_thread "lab01/lambda_thread.cpp")
add_executable (timing "lab01/timing.cpp")
add_executable (monte_carlo_pi "lab01/monte_carlo_pi.cpp")

# lab 2
add_executable (increment "lab02/increment.cpp")
add_executable (increment-mutex "lab02/increment-mutex.cpp")
add_executable (increment-guarded "lab02/increment-guarded.cpp")
add_executable (condition-variables "lab02/condition-variables.cpp")
add_executable (increment-oop "lab02/guarded.cpp" "lab02/guarded.h" "lab02/increment-oop.cpp")
add_executable (threadsafe-stack "lab02/threadsafe_stack.h" lab02/stack-test.cpp)
add_executable (increment-atomic lab02/increment_atomic.cpp)
add_executable (atomic-flag lab02/atomic_flag.cpp)
add_executable (vector-max-future lab02/vector_max_future.cpp)
add_executable (monte_carlo_promise lab02/monte_carlo_promise.cpp)
add_executable (mandelbrot_fractal lab02/mandelbrot_fractal.cpp)
add_executable (monte_carlo_atomic lab02/monte_carlo_atomic.cpp)
add_executable (deadlock lab02/deadlock.cpp)

# lab 3
add_executable (hello_openmp lab03/hello_openmp.cpp)
add_executable (pi_openmp lab03/pi_openmp.cpp)
add_executable (bubble_sort lab03/bubble_sort.cpp)
add_executable (parallel_sort lab03/parallel_sort.cpp)
add_executable (trapezoidal_rule lab03/trapezoidal_rule.cpp)
add_executable (scheduling lab03/scheduling.cpp)

#lab 4
add_executable (simd lab04/simd.cpp)
add_executable (simd_add lab04/simd_add.cpp)
add_executable (normalise_vector lab04/normalise_vector.cpp)

# lab 5
add_executable(hello lab05/hello.cpp)
target_link_libraries(hello ${MPI_LIBRARIES})

add_executable(send-receive lab05/send-receive.cpp)
target_link_libraries(send-receive ${MPI_LIBRARIES})

add_executable(monte_carlo_mpi lab05/monte_carlo_mpi.cpp)
target_link_libraries(monte_carlo_mpi ${MPI_LIBRARIES})

add_executable(scatter_gather lab05/scatter-gather.cpp)
target_link_libraries(scatter_gather ${MPI_LIBRARIES})

add_executable(broadcast lab05/broadcast.cpp)
target_link_libraries(broadcast ${MPI_LIBRARIES})

if(MPI_COMPILE_FLAGS)
  set_target_properties(hello PROPERTIES
    COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
  set_target_properties(hello PROPERTIES
    LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()

# lab 6
add_executable(mandelbrot_mpi lab06/mandelbrot_mpi.cpp)
target_link_libraries(mandelbrot_mpi ${MPI_LIBRARIES})

add_executable(sort_mpi lab06/sort_mpi.cpp)
target_link_libraries(sort_mpi ${MPI_LIBRARIES})

add_executable(trapezoidal_mpi lab06/trapezoidal_mpi.cpp)
target_link_libraries(trapezoidal_mpi ${MPI_LIBRARIES})

add_executable(latency lab06/latency.cpp)
target_link_libraries(latency ${MPI_LIBRARIES})

# lab 7
add_executable(init_opencl lab07/init_opencl.cpp)
add_executable(info_opencl lab07/info_opencl.cpp)

add_executable(vec_add_ocl lab07/vec_add_ocl.cpp)
configure_file(lab07/vec_add.cl vec_add.cl COPYONLY) # copy kernel file to build output

add_executable(mat_mult lab07/mat_mult.cpp)
configure_file(lab07/mat_mult.cl mat_mult.cl COPYONLY) # copy kernel file to build output

# lab 8
cuda_add_executable(cuda_info lab08/cuda_info.cu)
target_compile_features(cuda_info PUBLIC cxx_std_11)

cuda_add_executable(vecadd_cuda lab08/vecadd_cuda.cu)
target_compile_features(vecadd_cuda PUBLIC cxx_std_11)

cuda_add_executable(matmult_cuda lab08/matmult_cuda.cu)
target_compile_features(matmult_cuda PUBLIC cxx_std_11)