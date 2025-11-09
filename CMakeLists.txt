cmake_minimum_required(VERSION 3.15)
project(TreeSolutions LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

include_directories(headers)

add_executable(RB_tree_main src/RB_tree_main.cpp)
add_executable(Set_tree_main src/Set_tree_main.cpp)

target_compile_options(RB_tree_main PRIVATE -Wall -Wextra -Wpedantic)
target_compile_options(Set_tree_main PRIVATE -Wall -Wextra -Wpedantic)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(RB_tree_main PRIVATE -g -O0 -fsanitize=address,undefined)
    target_compile_options(Set_tree_main PRIVATE -g -O0 -fsanitize=address,undefined)
    
    target_link_options(RB_tree_main PRIVATE -fsanitize=address,undefined)
    target_link_options(Set_tree_main PRIVATE -fsanitize=address,undefined)
    
else()
    target_compile_options(RB_tree_main PRIVATE -O2 -DNDEBUG)
    target_compile_options(Set_tree_main PRIVATE -O2 -DNDEBUG)
endif()
