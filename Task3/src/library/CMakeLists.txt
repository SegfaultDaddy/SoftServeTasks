file(GLOB library_src CONFIGURE_DEPENDS "*.c" "*.cpp")
target_sources(task3_library PRIVATE ${library_src})