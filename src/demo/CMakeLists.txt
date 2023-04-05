cmake_minimum_required(VERSION 3.7)
project(demo)

set(CMAKE_CXX_STANDARD 14)


# header files.
include_directories(${CMAKE_SOURCE_DIR}/include)


# source files.
file(GLOB_RECURSE SRC_DIRS . *.*)


add_executable(demo ${SRC_DIRS})

target_link_libraries(demo strawbase strawgraphics)

