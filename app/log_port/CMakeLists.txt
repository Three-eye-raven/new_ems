aux_source_directory(. LOG_PORT_SOURCES)
add_library(log_port STATIC ${LOG_PORT_SOURCES})
target_link_libraries(log_port libzlog.a)
