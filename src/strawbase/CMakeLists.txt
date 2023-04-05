cmake_minimum_required(VERSION 3.7)
project(strawbase)

set(CMAKE_CXX_STANDARD 14)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


# header files.
include_directories(${CMAKE_SOURCE_DIR}/include)


# source files.
file(GLOB_RECURSE SRC_DIRS . *.*)

add_definitions(-dSTRAW_FRAMEWORK_API)


include(${CMAKE_SOURCE_DIR}/CMakeLists_base.txt)
generate_source_group_name(strawbase)


add_library(strawbase SHARED ${SRC_DIRS})
