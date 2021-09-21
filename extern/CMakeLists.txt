#### External libraries
cmake_minimum_required(VERSION 3.10)

### GLFW ###

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "")
set(GLFW_BUILD_TESTS OFF CACHE BOOL "")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "")
set(GLFW_INSTALL OFF CACHE BOOL "")
add_subdirectory(glfw)

### GLAD ###

add_library(glad glad/src/gl.c)
target_include_directories(glad PRIVATE glad/include)
