aux_source_directory(. PVPCS_HW_150KW_SOURCES)
add_library(pvpcs_hw_150kw STATIC ${PVPCS_HW_150KW_SOURCES})
target_link_libraries(pvpcs_hw_150kw modbus_master)
