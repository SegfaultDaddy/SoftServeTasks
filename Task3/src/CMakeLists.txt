add_library(thread_task3_library)
add_subdirectory(thread)
target_include_directories(thread_task3_library PUBLIC ${PROJECT_SOURCE_DIR}/include/thread)

add_library(reader_task3_library)
add_subdirectory(reader)
target_link_libraries(reader_task3_library PRIVATE thread_task3_library)
target_include_directories(reader_task3_library PUBLIC ${PROJECT_SOURCE_DIR}/include/reader)

add_executable(task3 main.cpp)
target_link_libraries(task3 PRIVATE reader_task3_library thread_task3_library)