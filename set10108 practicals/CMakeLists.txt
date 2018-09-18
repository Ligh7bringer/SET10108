# CMakeList.txt : CMake project for set10108 practicals, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (hello_world "lab01/hello_world.cpp" "lab01/hello_world.h")
add_executable (multiple_tasks "lab01/multiple_tasks.cpp")
add_executable (parameter_thread "lab01/parameter_thread.cpp")
add_executable (lambda "lab01/lambda.cpp")
add_executable (lambda_thread "lab01/lambda_thread.cpp")
add_executable (timing "lab01/timing.cpp")
add_executable (monte_carlo_pi "lab01/monte_carlo_pi.cpp")
