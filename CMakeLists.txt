cmake_minimum_required(VERSION 3.10)

project(CMakeExample)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}" "${CMAKE_MODULE_PATH}")
set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}" "${CMAKE_PREFIX_PATH}")

option(BUILD_TESTS "Build tests" ON)

enable_testing()

add_subdirectory(src)

if (BUILD_TESTS)
    add_subdirectory(test)
endif ()