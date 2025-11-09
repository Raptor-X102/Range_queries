cmake_minimum_required(VERSION 3.15)
project(TreeTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

include_directories(../headers)

add_executable(TreeTests src/test_main.cpp)

target_compile_features(TreeTests PRIVATE cxx_std_17)

target_compile_options(TreeTests PRIVATE -Wall -Wextra -Wpedantic)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(TreeTests PRIVATE -g -O0 -fsanitize=address,undefined)
    target_link_options(TreeTests PRIVATE -fsanitize=address,undefined)
else()
    target_compile_options(TreeTests PRIVATE -O2 -DNDEBUG)
endif()
