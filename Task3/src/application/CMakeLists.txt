file(GLOB application_src CONFIGURE_DEPENDS "*.c" "*.cpp")
target_sources(task3 PRIVATE ${application_src})