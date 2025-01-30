cmake_minimum_required(VERSION 3.30.0)
project(task3)
set(CMAKE_CXX_STANDARD 23)

include(FetchContent)
FetchContent_Declare(ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
    GIT_TAG v5.0.0
)

FetchContent_MakeAvailable(ftxui)

find_package(GTest 1.15.2 CONFIG)
if(NOT GTest_FOUND)

FetchContent_Declare(googletest
                     GIT_REPOSITORY https://github.com/google/googletest.git
                     GIT_TAG v1.15.2)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

endif()

add_subdirectory(src)
add_subdirectory(test)