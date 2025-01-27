add_library(task2_library)
add_subdirectory(any_type)
target_include_directories(task2_library PUBLIC ${PROJECT_SOURCE_DIR}/include)