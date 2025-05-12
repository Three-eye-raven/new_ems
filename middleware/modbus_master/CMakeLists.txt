aux_source_directory(. MODBUS_MASTER_SOURCES)
add_library(modbus_master STATIC ${MODBUS_MASTER_SOURCES})
target_link_libraries(modbus_master libmodbus.a m)


