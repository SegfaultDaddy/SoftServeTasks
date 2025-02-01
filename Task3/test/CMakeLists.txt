enable_testing()

add_executable(reader_test reader_test.cpp)
target_link_libraries(reader_test PRIVATE reader_task3_library GTest::gtest_main)

add_executable(thread_test thread_test.cpp)
target_link_libraries(thread_test PRIVATE thread_task3_library GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(reader_test thread_test)