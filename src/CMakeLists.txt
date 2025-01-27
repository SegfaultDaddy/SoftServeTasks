add_library(task3_library library/file_reader.cpp library/counter.cpp)
target_include_directories(task3_library PUBLIC ${PROJECT_SOURCE_DIR}/include/library)

add_executable(task3 application/main.cpp)
target_link_libraries(task3 PRIVATE task3_library)