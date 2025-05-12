aux_source_directory(. EMS_MODBUS_SLAVE)
add_library(ems_modbus_slave STATIC ${EMS_MODBUS_SLAVE})
target_link_libraries(ems_modbus_slave libevent.a modbus_slave)


