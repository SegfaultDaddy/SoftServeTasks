cmake_minimum_required(VERSION 3.30.0)
project(task3)
set(CMAKE_CXX_STANDARD 23)

find_package(GTest 1.15.2 CONFIG)

if(NOT GTest_FOUND)

include(FetchContent)
FetchContent_Declare(googletest
                     GIT_REPOSITORY https://github.com/google/googletest.git
                     GIT_TAG v1.15.2)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

endif()

add_subdirectory(src)
add_subdirectory(test)