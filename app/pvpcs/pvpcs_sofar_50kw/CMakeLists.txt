aux_source_directory(. PVPCS_SOFAR_50KW_SOURCES)
add_library(pvpcs_sofar_50kw STATIC ${PVPCS_SOFAR_50KW_SOURCES})
target_link_libraries(pvpcs_sofar_50kw modbus_master)
