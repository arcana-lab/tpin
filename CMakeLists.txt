cmake_minimum_required(VERSION 3.10)

# Set the compiler to clang++ before project declaration
set(CMAKE_CXX_COMPILER "clang++")

project(tpin)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find OpenMP
find_package(OpenMP REQUIRED)

# Create executable
add_executable(tpin tpin.cpp)

# Link OpenMP
target_link_libraries(tpin OpenMP::OpenMP_CXX)

# Enable compiler-specific flags for clang++
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    target_compile_options(tpin PRIVATE -Wall -Wextra -O2)
endif()