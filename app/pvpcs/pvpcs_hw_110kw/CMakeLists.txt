aux_source_directory(. PVPCS_HW_110KW_SOURCES)
add_library(pvpcs_hw_110kw STATIC ${PVPCS_HW_110KW_SOURCES})
target_link_libraries(pvpcs_hw_110kw modbus_master)
