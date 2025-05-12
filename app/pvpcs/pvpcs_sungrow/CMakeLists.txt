aux_source_directory(. PVPCS_SUNGROW_SOURCES)
add_library(pvpcs_sungrow STATIC ${PVPCS_SUNGROW_SOURCES})
target_link_libraries(pvpcs_sungrow modbus_master)
