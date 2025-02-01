file(GLOB reader_src CONFIGURE_DEPENDS "*.c" "*.cpp")

target_sources(reader_task3_library PRIVATE ${reader_src})