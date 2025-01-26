#add_library(task3_library)
#target_include_directories(task3_library PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(task3 main.cpp)
target_include_directories(task3 PUBLIC ${PROJECT_SOURCE_DIR}/include)