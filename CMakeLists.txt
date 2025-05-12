cmake_minimum_required(VERSION 3.16)
project(new_ems C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../build/lib)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/../build)

#add_compile_options(-std=c11)

#external libraries includes
include_directories(${PROJECT_SOURCE_DIR}/external/libmodbus/include/modbus)
include_directories(${PROJECT_SOURCE_DIR}/external/libzlog/include)
include_directories(${PROJECT_SOURCE_DIR}/external/libiniparser/include)
include_directories(${PROJECT_SOURCE_DIR}/external/libsocketcan/include)
include_directories(${PROJECT_SOURCE_DIR}/external/libevent/include)

#middleware libraries includes
include_directories(${PROJECT_SOURCE_DIR}/middleware/ring_lockfree_sync_queue/include)
include_directories(${PROJECT_SOURCE_DIR}/middleware/modbus_master/include)
include_directories(${PROJECT_SOURCE_DIR}/middleware/modbus_slave/include)

#device libraries includes

#app libraries includes
include_directories(${PROJECT_SOURCE_DIR}/app/log_port/include)
include_directories(${PROJECT_SOURCE_DIR}/app/atomic_test/include)
include_directories(${PROJECT_SOURCE_DIR}/app/pvpcs/pvpcs_data_interaction)
include_directories(${PROJECT_SOURCE_DIR}/app/pvpcs/pvpcs_hw_110kw)
include_directories(${PROJECT_SOURCE_DIR}/app/pvpcs/pvpcs_hw_150kw)
include_directories(${PROJECT_SOURCE_DIR}/app/pvpcs/pvpcs_sofar_50kw)
include_directories(${PROJECT_SOURCE_DIR}/app/pvpcs/pvpcs_sungrow)
include_directories(${PROJECT_SOURCE_DIR}/app/pvpcs/include)
include_directories(${PROJECT_SOURCE_DIR}/app/pvpcs/pvpcs_data_map)
include_directories(${PROJECT_SOURCE_DIR}/app/ems_modbus_slave)

#external libraries linking
link_directories(external/libmodbus/lib)
link_directories(external/libzlog/lib)
link_directories(external/libiniparser/lib)
link_directories(${PROJECT_SOURCE_DIR}/external/libsocketcan/lib)
link_directories(${PROJECT_SOURCE_DIR}/external/libevent/lib)

#middleware libraries linking
add_subdirectory(middleware/modbus_master)
add_subdirectory(middleware/modbus_slave)
add_subdirectory(${PROJECT_SOURCE_DIR}/middleware/ring_lockfree_sync_queue)

#app libraries linking
add_subdirectory(app/log_port)
add_subdirectory(app/atomic_test)
add_subdirectory(app/pvpcs/pvpcs_data_interaction)
add_subdirectory(app/pvpcs/pvpcs_hw_110kw)
add_subdirectory(app/pvpcs/pvpcs_hw_150kw)
add_subdirectory(app/pvpcs/pvpcs_sofar_50kw)
add_subdirectory(app/pvpcs/pvpcs_sungrow)
add_subdirectory(app/pvpcs)

add_subdirectory(app/pvpcs/pvpcs_data_map)

add_subdirectory(${PROJECT_SOURCE_DIR}/app/ems_modbus_slave)

aux_source_directory(./ MAIN_SOURCES)

add_executable(new_ems ${MAIN_SOURCES})

#library linking
target_link_libraries(new_ems ems_modbus_slave libevent.a libsocketcan.a libmodbus.a m pthread rt)