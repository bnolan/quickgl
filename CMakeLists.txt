cmake_minimum_required(VERSION 3.15) # Set the minimum required version of cmake

# Set your project name and version
project(QuickGL VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(QuickGL src/main.cpp)

# Include directories
target_include_directories(QuickGL PUBLIC "${PROJECT_BINARY_DIR}" include/)
