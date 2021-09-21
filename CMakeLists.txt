cmake_minimum_required(VERSION 3.10)
project(raytracer)

# Use C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Specify output binary directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Build external libraries
add_subdirectory(extern/)

# Set the root for include paths
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/src
	extern/glad/include/
	extern/glfw/
)

# Build our project
add_executable(${PROJECT_NAME} src/raytracer.cpp)

# Link
target_link_libraries(${PROJECT_NAME}
	glad
	glfw
)
