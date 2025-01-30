add_library(task3_library)
add_subdirectory(library)
target_include_directories(task3_library PUBLIC ${PROJECT_SOURCE_DIR}/include/library)

add_executable(task3)
add_subdirectory(application)
target_link_libraries(task3 PRIVATE task3_library)
target_include_directories(task3 PRIVATE ${PROJECT_SOURCE_DIR}/include/application)