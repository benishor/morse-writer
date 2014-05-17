cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
project (morse-writer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g")

add_subdirectory(src)
add_subdirectory(tests)

add_custom_target(test tests/main DEPENDS tests/main)