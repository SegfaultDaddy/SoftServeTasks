file(GLOB library_src CONFIGURE_DEPENDS "*.c" "*.cpp")
target_sources(thread_task3_library PRIVATE ${library_src})