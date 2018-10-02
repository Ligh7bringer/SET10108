# CMakeList.txt : CMake project for set10108 practicals, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Use OpenMP
include(FindOpenMP)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")

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
add_executable(increment-atomic lab02/increment_atomic.cpp)
add_executable(atomic-flag lab02/atomic_flag.cpp)
add_executable(vector-max-future lab02/vector_max_future.cpp)
add_executable(monte_carlo_promise lab02/monte_carlo_promise.cpp)
add_executable(mandelbrot_fractal lab02/mandelbrot_fractal.cpp)
add_executable(monte_carlo_atomic lab02/monte_carlo_atomic.cpp)

# lab 3
add_executable(hello_openmp lab03/hello_openmp.cpp)
add_executable(pi_openmp lab03/pi_openmp.cpp)
add_executable(bubble_sort lab03/bubble_sort.cpp)
add_executable(parallel_sort lab03/parallel_sort.cpp)
add_executable(trapezoidal_rule lab03/trapezoidal_rule.cpp)
add_executable(scheduling lab03/scheduling.cpp)