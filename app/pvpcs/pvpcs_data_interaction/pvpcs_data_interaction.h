//
// Created by 谷子 on 2025/4/7.
//

#ifndef PVPCS_DATA_INTERACTION_H
#define PVPCS_DATA_INTERACTION_H

#include <stdint.h>
#include <stdatomic.h>

#define PVPCS_PV_MAX_NUM 32

#define PVPCS_WATING_STATE 0
#define PVPCS_IDLE_STATE 1
#define PVPCS_GRID_CONNECTED_OPERATION_STATE 2
#define PVPCS_GRID_DISCONNECTED_OPERATION_STATE 3
#define PVPCS_ERROR_STATE 4

typedef struct{
    atomic_ushort work_mode;
    atomic_uint daily_power_generation;
    atomic_uint total_power_generation;
    atomic_uint run_time;
    atomic_int total_input_power;
    atomic_int total_active_power;
    atomic_int total_reactive_power;
    atomic_int total_apparent_power;
    atomic_short temperature;
    atomic_short power_factor;
    atomic_ushort efficiency;
    atomic_short reactor_temperature;
    atomic_short igbt_temperature;
    atomic_short pv_current[32];
    atomic_ushort pv_voltage[32];
    atomic_ushort phase_voltage[3];
    atomic_short phase_current[3];
    atomic_ushort warning;
    atomic_uint fault;
    atomic_ushort sn[16];
    atomic_bool except_power_on_off_status;//0关，1开
    atomic_uint except_active_power;//0.1kw
    atomic_uchar active_power_command_mode;//有功功率设置模式
    atomic_uint max_active_power;//0.1kw
}pvpcs_single_data_t;//单台设备的数据

typedef struct{
    atomic_bool connect_state[32];
    atomic_uint connect_status;
    atomic_uint daily_power_generation;
    atomic_uint total_power_generation;
    atomic_ushort active_power;
    atomic_uint except_active_power;
    atomic_uint max_active_power;
}pvpcs_multi_data_t;//汇总数据

#endif //PVPCS_DATA_INTERACTION_H
