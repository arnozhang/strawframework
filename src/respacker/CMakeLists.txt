cmake_minimum_required(VERSION 3.7)
project(respacker)

set(CMAKE_CXX_STANDARD 14)


# header files.
include_directories(${CMAKE_SOURCE_DIR}/include)


# source files.
file(GLOB_RECURSE SRC_DIRS . *.*)


add_executable(respacker ${SRC_DIRS})

target_link_libraries(respacker strawbase)
