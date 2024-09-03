# CMake version requirement
cmake_minimum_required(VERSION 3.10)

# Project name
project(garbage_collector VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories (header files)
include_directories(include)

# Enable compiler warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# Find all .cpp files in the src directory and add them as sources
file(GLOB_RECURSE SOURCES "src/*.cpp")

# Add the executable
add_executable(garbage_collector ${SOURCES})


# Specify the output directory for the build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
