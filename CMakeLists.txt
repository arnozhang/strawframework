cmake_minimum_required(VERSION 3.7)
project(strawframework)

set(CMAKE_CXX_STANDARD 14)


include_directories(./include)
include_directories(./include/thirdparty/skia/config)
include_directories(./include/thirdparty/skia/core)

if (WIN32)
    #include_directories("C:\\Program Files (x86)\\Microsoft Visual Studio 12.0\\VC\\include")
endif ()

add_definitions(-DSTRAW_DYNAMIC)
add_definitions(-DUNICODE)


# platform.
if (WIN32)
    add_definitions(-DSTRAW_BUILD_FOR_WIN32)
elseif (APPLE)
    add_definitions(-DSTRAW_BUILD_FOR_MAC)
elseif (UNIX)
    add_definitions(-DSTRAW_BUILD_FOR_LINUX)
endif ()


# debug & release.
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    add_definitions(-DDEBUG -D_DEBUG)
else ()
    add_definitions(-DNDEBUG)
endif ()


set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/build/lib)


add_subdirectory(src/strawbase)
add_subdirectory(src/strawgraphics)
add_subdirectory(src/demo)
add_subdirectory(src/respacker)
add_subdirectory(src/plugingenerator)
