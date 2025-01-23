enable_testing()

add_executable(test any_type_test.cpp)
target_link_libraries(test PRIVATE task2_library GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test)