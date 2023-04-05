cmake_minimum_required(VERSION 3.7)
project(strawgraphics)

set(CMAKE_CXX_STANDARD 14)



# header files.
include_directories(${CMAKE_SOURCE_DIR}/include)


# source files.
file(GLOB_RECURSE SRC_DIRS . *.*)

add_definitions(-dSTRAW_FRAMEWORK_API)


add_library(strawgraphics SHARED ${SRC_DIRS})
