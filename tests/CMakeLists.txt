cmake_minimum_required(VERSION 3.15)
project(TreeTests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
endif()

# Подключаем путь к заголовкам в корне проекта
include_directories(../headers)

add_executable(TreeTests src/test_main.cpp)

target_compile_features(TreeTests PRIVATE cxx_std_17)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(TreeTests PRIVATE -Wall -Wextra -Wpedantic)
endif()
