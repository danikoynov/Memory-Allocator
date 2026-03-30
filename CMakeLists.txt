cmake_minimum_required(VERSION 3.16)

project(toyalloc LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(allocator
    src/allocator.cpp
)

target_include_directories(allocator
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_features(allocator PUBLIC cxx_std_20)

enable_testing()

add_subdirectory(tests)
