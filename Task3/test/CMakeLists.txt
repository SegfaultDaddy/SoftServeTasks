enable_testing()

add_executable(test test.cpp)
target_link_libraries(test PRIVATE task3_library GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test)