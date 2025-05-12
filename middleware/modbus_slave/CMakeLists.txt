aux_source_directory(. MODBUS_SLAVE_SOURCES)
add_library(modbus_slave STATIC ${MODBUS_SLAVE_SOURCES})
target_link_libraries(modbus_slave ring_lockfree_sync_queue libmodbus.a)


