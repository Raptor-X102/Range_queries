cmake_minimum_required(VERSION 3.15)
project(TreeSolutions LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

include_directories(headers)

# RB_tree программа
add_executable(RB_tree_main src/RB_tree_main.cpp)

# Set_tree программа  
add_executable(Set_tree_main src/Set_tree_main.cpp)

# Настройки компилятора
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(RB_tree_main PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(Set_tree_main PRIVATE -Wall -Wextra -Wpedantic)
endif()
