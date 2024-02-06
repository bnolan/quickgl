cmake_minimum_required(VERSION 3.15) # Set the minimum required version of cmake

# Set your project name and version
project(QuickGL VERSION 1.0 LANGUAGES CXX)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

# Add SDL2
find_package(SDL2 REQUIRED)
find_package(SDL2TTF REQUIRED)

# Add the executable
add_executable(QuickGL src/main.cpp)

# Add QuickJS
add_library(QuickJS STATIC IMPORTED)
set_target_properties(QuickJS PROPERTIES
    IMPORTED_LOCATION "${PROJECT_SOURCE_DIR}/third_party/quickjs/libquickjs.a"
    INTERFACE_INCLUDE_DIRECTORIES "${PROJECT_SOURCE_DIR}/third_party/quickjs")

# Link QuickJS with your project
target_link_libraries(QuickGL 
  QuickJS 
  ${SDL2_LIBRARIES}
  ${SDL2TTF_LIBRARY}
  )

# Include directories
target_include_directories(QuickGL PUBLIC 
  "${PROJECT_BINARY_DIR}" 
  include/ 
  "${PROJECT_SOURCE_DIR}/third_party/quickjs"
  ${SDL2_INCLUDE_DIRS}
  ${SDL2TTF_INCLUDE_DIR}
)
