# CMakeList.txt : CMake project for set10108 practicals, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
#lab 1
add_executable (hello_world "lab01/hello_world.cpp" "lab01/hello_world.h")
add_executable (multiple_tasks "lab01/multiple_tasks.cpp")
add_executable (parameter_thread "lab01/parameter_thread.cpp")
add_executable (lambda "lab01/lambda.cpp")
add_executable (lambda_thread "lab01/lambda_thread.cpp")
add_executable (timing "lab01/timing.cpp")
add_executable (monte_carlo_pi "lab01/monte_carlo_pi.cpp")

#lab2
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
