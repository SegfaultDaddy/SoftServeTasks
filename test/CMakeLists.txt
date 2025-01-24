enable_testing()

add_executable(test string_list_test.cpp)
target_link_libraries(test PRIVATE task1_library GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test)