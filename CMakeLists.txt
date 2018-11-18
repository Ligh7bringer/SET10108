# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.11)

# set c++ standard
set (CMAKE_CXX_STANDARD 11)

project ("set10108 practicals")

# Include sub-projects.
add_subdirectory ("labs")
