enable_testing()

add_executable(any_type_test any_type_test.cpp)
target_link_libraries(any_type_test PRIVATE task2_library GTest::gtest_main)

add_executable(variant_type_test variant_type_test.cpp)
target_link_libraries(variant_type_test PRIVATE task2_library GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(any_type_test variant_type_test)