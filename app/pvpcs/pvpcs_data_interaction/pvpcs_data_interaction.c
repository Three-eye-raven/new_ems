//
// Created by 谷子 on 2025/4/7.
//
#include "pvpcs_data_interaction.h"
#include "pvpcs_data_interaction_api.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#define PVPCS_MAX_NUM 32

static pvpcs_single_data_t pvpcs_single_data[PVPCS_MAX_NUM] = {
    [0] = {
        .except_power_on_off_status =  true,
        .except_active_power = 400,
    },
    [1] = {
        .except_power_on_off_status =  true,
        .except_active_power = 400,
    },
    [2] = {
        .except_power_on_off_status =  true,
        .except_active_power = 500,
    },
};

static pvpcs_multi_data_t pvpcs_multi_data = {
    .connect_state          = {0},
    .connect_status         = 0,
    .daily_power_generation = 0,
    .total_power_generation = 0,
    .active_power     = 0,
    .max_active_power = 0,
};

// 数值交互接口

uint16_t pvpcs_work_mode_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const uint16_t work_mode = atomic_load(&pvpcs_single_data[device_id].work_mode);
    return work_mode;
}
void pvpcs_work_mode_set(const uint16_t device_id, uint16_t work_mode)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].work_mode, work_mode);
}

uint32_t pvpcs_daily_power_generation_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const uint32_t daily_power_generation =
        atomic_load(&pvpcs_single_data[device_id].daily_power_generation);
    return daily_power_generation;
}
void pvpcs_daily_power_generation_set(const uint16_t device_id, uint32_t daily_power_generation)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].daily_power_generation, daily_power_generation);
}

uint32_t pvpcs_total_power_generation_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const uint32_t total_power_generation =
        atomic_load(&pvpcs_single_data[device_id].total_power_generation);
    return total_power_generation;
}
void pvpcs_total_power_generation_set(const uint16_t device_id, uint32_t total_power_generation)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].total_power_generation, total_power_generation);
}

uint32_t pvpcs_run_time_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const uint32_t run_time = atomic_load(&pvpcs_single_data[device_id].run_time);
    return run_time;
}
void pvpcs_run_time_set(const uint16_t device_id, uint32_t run_time)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].run_time, run_time);
}

int32_t pvpcs_total_input_power_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const int32_t total_input_power = atomic_load(&pvpcs_single_data[device_id].total_input_power);
    return total_input_power;
}
void pvpcs_total_input_power_set(const uint16_t device_id, int32_t total_input_power)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].total_input_power, total_input_power);
}

int32_t pvpcs_total_active_power_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const int32_t total_active_power =
        atomic_load(&pvpcs_single_data[device_id].total_active_power);
    return total_active_power;
}
void pvpcs_total_active_power_set(const uint16_t device_id, int32_t total_active_power)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].total_active_power, total_active_power);
}

int32_t pvpcs_total_reactive_power_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const int32_t total_reactive_power =
        atomic_load(&pvpcs_single_data[device_id].total_reactive_power);
    return total_reactive_power;
}
void pvpcs_total_reactive_power_set(const uint16_t device_id, int32_t total_reactive_power)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].total_reactive_power, total_reactive_power);
}

int32_t pvpcs_total_apparent_power_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const int32_t total_apparent_power =
        atomic_load(&pvpcs_single_data[device_id].total_apparent_power);
    return total_apparent_power;
}
void pvpcs_total_apparent_power_set(const uint16_t device_id, int32_t total_apparent_power)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].total_apparent_power, total_apparent_power);
}

int16_t pvpcs_temperature_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const int16_t temperature = atomic_load(&pvpcs_single_data[device_id].temperature);
    return temperature;
}
void pvpcs_temperature_set(const uint16_t device_id, int16_t temperature)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].temperature, temperature);
}

int16_t pvpcs_power_factor_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const int16_t power_factor = atomic_load(&pvpcs_single_data[device_id].power_factor);
    return power_factor;
}
void pvpcs_power_factor_set(const uint16_t device_id, int16_t power_factor)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].power_factor, power_factor);
}

uint16_t pvpcs_efficiency_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const uint16_t efficiency = atomic_load(&pvpcs_single_data[device_id].efficiency);
    return efficiency;
}
void pvpcs_efficiency_set(const uint16_t device_id, uint16_t efficiency)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].efficiency, efficiency);
}

int16_t pvpcs_reactor_temperature_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const int16_t reactor_temperature =
        atomic_load(&pvpcs_single_data[device_id].reactor_temperature);
    return reactor_temperature;
}
void pvpcs_reactor_temperature_set(const uint16_t device_id, int16_t reactor_temperature)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].reactor_temperature, reactor_temperature);
}

int16_t pvpcs_igbt_temperature_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const int16_t igbt_temperature = atomic_load(&pvpcs_single_data[device_id].igbt_temperature);
    return igbt_temperature;
}
void pvpcs_igbt_temperature_set(const uint16_t device_id, int16_t igbt_temperature)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].igbt_temperature, igbt_temperature);
}

int16_t pvpcs_pv_current_get(const uint16_t device_id, uint16_t pv_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    if (pv_id < PVPCS_PV_MAX_NUM) {
        const int16_t pv_current = atomic_load(&pvpcs_single_data[device_id].pv_current[pv_id]);
        return pv_current;
    }
    return 0;
}
void pvpcs_pv_current_set(const uint16_t device_id, uint16_t pv_id, int16_t current)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    if (pv_id < PVPCS_PV_MAX_NUM) {
        atomic_store(&pvpcs_single_data[device_id].pv_current[pv_id], current);
    }
}

uint16_t pvpcs_pv_voltage_get(const uint16_t device_id, uint16_t pv_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    if (pv_id < PVPCS_PV_MAX_NUM) {
        const uint16_t pv_voltage = atomic_load(&pvpcs_single_data[device_id].pv_voltage[pv_id]);
        return pv_voltage;
    }
    return 0;
}
void pvpcs_pv_voltage_set(const uint16_t device_id, uint16_t pv_id, uint16_t voltage)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    if (pv_id < PVPCS_PV_MAX_NUM) {
        atomic_store(&pvpcs_single_data[device_id].pv_voltage[pv_id], voltage);
    }
}

uint16_t pvpcs_phase_a_voltage_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const uint16_t voltage = atomic_load(&pvpcs_single_data[device_id].phase_voltage[0]);
    return voltage;
}
void pvpcs_phase_a_voltage_set(const uint16_t device_id, uint16_t voltage)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].phase_voltage[0], voltage);
}

uint16_t pvpcs_phase_b_voltage_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const uint16_t voltage = atomic_load(&pvpcs_single_data[device_id].phase_voltage[1]);
    return voltage;
}
void pvpcs_phase_b_voltage_set(const uint16_t device_id, uint16_t voltage)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].phase_voltage[1], voltage);
}

uint16_t pvpcs_phase_c_voltage_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const uint16_t voltage = atomic_load(&pvpcs_single_data[device_id].phase_voltage[2]);
    return voltage;
}
void pvpcs_phase_c_voltage_set(const uint16_t device_id, uint16_t current)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].phase_voltage[2], current);
}

int16_t pvpcs_phase_a_current_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const int16_t current = atomic_load(&pvpcs_single_data[device_id].phase_current[0]);
    return current;
}
void pvpcs_phase_a_current_set(const uint16_t device_id, int16_t current)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].phase_current[0], current);
}

int16_t pvpcs_phase_b_current_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const int16_t current = atomic_load(&pvpcs_single_data[device_id].phase_current[1]);
    return current;
}
void pvpcs_phase_b_current_set(const uint16_t device_id, int16_t current)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].phase_current[1], current);
}

int16_t pvpcs_phase_c_current_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const int16_t current = atomic_load(&pvpcs_single_data[device_id].phase_current[2]);
    return current;
}
void pvpcs_phase_c_current_set(const uint16_t device_id, int16_t current)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].phase_current[2], current);
}

uint16_t pvpcs_warning_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const uint16_t warning = atomic_load(&pvpcs_single_data[device_id].warning);
    return warning;
}
void pvpcs_warning_set(const uint16_t device_id, uint16_t warning)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].warning, warning);
}

uint32_t pvpcs_fault_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const uint32_t fault = atomic_load(&pvpcs_single_data[device_id].fault);
    return fault;
}
void pvpcs_fault_set(const uint16_t device_id, uint32_t fault)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].fault, fault);
}

void pvpcs_sn_get(uint16_t device_id, uint16_t* sn, uint8_t len)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    if (sn == NULL) {
        return;
    }
    for (uint8_t i = 0; i < len; i++) {
        sn[i] = atomic_load(&pvpcs_single_data[device_id].sn[i]);
    }
}
void pvpcs_sn_set(uint16_t device_id, uint16_t* sn, uint8_t len)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    if (sn == NULL) {
        return;
    }
    for (uint8_t i = 0; i < len; i++) {
        atomic_store(&pvpcs_single_data[device_id].sn[i], sn[i]);
    }
}

bool pvpcs_except_power_on_off_status_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const bool except_power_on_off_status =
        atomic_load(&pvpcs_single_data[device_id].except_power_on_off_status);
    return except_power_on_off_status;
}
void pvpcs_except_power_on_off_status_set(const uint16_t device_id, bool on)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].except_power_on_off_status, on);
}

uint32_t pvpcs_except_active_power_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const uint32_t except_active_power =
        atomic_load(&pvpcs_single_data[device_id].except_active_power);
    return except_active_power;
}
void pvpcs_except_active_power_set(const uint16_t device_id, uint32_t active_power)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].except_active_power, active_power);
}

uint8_t pvpcs_active_power_command_mode_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const uint8_t active_power_command =
        atomic_load(&pvpcs_single_data[device_id].active_power_command_mode);
    return active_power_command;
}
void pvpcs_active_power_command_mode_set(const uint16_t device_id, uint8_t mode)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].active_power_command_mode, mode);
}

uint32_t pvpcs_max_active_power_get(const uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return 0;
    }
    const uint32_t max_active_power = atomic_load(&pvpcs_single_data[device_id].max_active_power);
    return max_active_power;
}
void pvpcs_max_active_power_set(const uint16_t device_id, uint32_t max_active_power)
{
    if (device_id >= PVPCS_MAX_NUM) {
        return;
    }
    atomic_store(&pvpcs_single_data[device_id].max_active_power, max_active_power);
}

bool pvpcs_connect_state_get(const uint16_t device_id)
{
    if (device_id < PVPCS_MAX_NUM) {
        const bool connect_state = atomic_load(&pvpcs_multi_data.connect_state[device_id]);
        return connect_state;
    }
    return false;
}
void pvpcs_connect_state_set(const uint16_t device_id, bool connect_state)
{
    if (device_id < PVPCS_MAX_NUM) {
        atomic_store(&pvpcs_multi_data.connect_state[device_id], connect_state);
    }
}

uint32_t pvpcs_connect_status_get(void)
{
    const uint32_t conn_status = atomic_load(&pvpcs_multi_data.connect_status);
    return conn_status;
}
void pvpcs_connect_status_set(uint32_t connect_status)
{
    atomic_store(&pvpcs_multi_data.connect_status, connect_status);
}

uint32_t pvpcs_multi_daily_power_generation_get(void)
{
    const uint32_t daily_power_generation = atomic_load(&pvpcs_multi_data.daily_power_generation);
    return daily_power_generation;
}
void pvpcs_multi_daily_power_generation_set(uint32_t daily_power_generation)
{
    atomic_store(&pvpcs_multi_data.daily_power_generation, daily_power_generation);
}

uint32_t pvpcs_multi_total_power_generation_get(void)
{
    const uint32_t total_power_generation = atomic_load(&pvpcs_multi_data.total_power_generation);
    return total_power_generation;
}
void pvpcs_multi_total_power_generation_set(uint32_t total_power_generation)
{
    atomic_store(&pvpcs_multi_data.total_power_generation, total_power_generation);
}

uint16_t pvpcs_multi_active_power_get(void)
{
    const uint16_t total_active_power = atomic_load(&pvpcs_multi_data.active_power);
    return total_active_power;
}
void pvpcs_multi_active_power_set(uint16_t total_active_power)
{
    atomic_store(&pvpcs_multi_data.active_power, total_active_power);
}

uint32_t pvpcs_multi_except_active_power_get(void)
{
    uint32_t total_except_active_power = atomic_load(&pvpcs_multi_data.except_active_power);
    return total_except_active_power;
}
void pvpcs_multi_except_active_power_set(uint32_t except_active_power)
{
    atomic_store(&pvpcs_multi_data.except_active_power, except_active_power);
}

uint32_t pvpcs_multi_max_active_power_get(void)
{
    const uint32_t max_active_power = atomic_load(&pvpcs_multi_data.max_active_power);
    return max_active_power;
}
void pvpcs_multi_max_active_power_set(uint32_t total_max_active_power)
{
    atomic_store(&pvpcs_multi_data.max_active_power, total_max_active_power);
}


// 逻辑功能交互接口

void pvpcs_loss_update(uint16_t device_id)
{
    pvpcs_connect_state_set(device_id, false);
}

void pvpcs_connect_state_update(uint16_t device_id)
{
    pvpcs_connect_state_set(device_id, true);
}

bool pvpcs_single_if_availability(uint16_t device_id)
{
    bool     online_state = pvpcs_connect_state_get(device_id);
    uint16_t work_mode    = pvpcs_work_mode_get(device_id);
    bool     availability = false;
    if (work_mode != 2 || online_state == true) {
        // 设备不在故障状态，且在线，则设备可用
        availability = true;
    }
    return availability;
}

uint16_t pvpcs_availability_num_get(void)
{
    uint16_t num = 0;
    for (uint8_t idx = 0; idx < PVPCS_MAX_NUM; idx++) {
        if (pvpcs_single_if_availability(idx) == true) {
            num++;
        }
    }
    return num;
}

void pvpcs_active_power_distribution(uint32_t except_active_power)
{
    uint16_t device_num = pvpcs_availability_num_get();   // 获取能够正常工作的设备数量
    uint32_t average_active_power = except_active_power / device_num;
    uint32_t max_power            = 0;
    uint32_t temp_power           = 0;
    for (uint8_t idx = 0; idx < PVPCS_MAX_NUM; idx++) {
        if (pvpcs_connect_state_get(idx) == true) {
            max_power = pvpcs_max_active_power_get(idx);
            if (max_power < average_active_power) {
                temp_power = max_power;
            }
            else {
                temp_power = average_active_power;
            }
            pvpcs_except_active_power_set(idx, temp_power);
            except_active_power -= temp_power;
        }
    }
}


// 数据打印接口

void print_pvpcs_single_data(uint16_t device_id)
{
    if (device_id >= PVPCS_MAX_NUM) {
        printf("Error: Null data pointer!\n");
        return;
    }
    pvpcs_single_data_t* data = &pvpcs_single_data[device_id];

    printf("===== PVPCS%d Single Device Data =====\n",device_id);

    // 基础参数
    printf("Work Mode: %hu\n", atomic_load(&data->work_mode));
    printf("Daily Generation: %u kWh\n", atomic_load(&data->daily_power_generation));
    printf("Total Generation: %u kWh\n", atomic_load(&data->total_power_generation));
    printf("Runtime: %u hours\n", atomic_load(&data->run_time));

    // 电力参数
    printf("--- Power Parameters ---");
    printf("Input Power: %d 0.1kW\n", atomic_load(&data->total_input_power));
    printf("Active Power: %d 0.1kW\n", atomic_load(&data->total_active_power));
    printf("Reactive Power: %d kVAR\n", atomic_load(&data->total_reactive_power));
    printf("Apparent Power: %d kVA\n", atomic_load(&data->total_apparent_power));

    // 温度监控
    printf("--- Temperature Monitoring ---\n");
    printf("Device Temp: %hd 0.1°C\n", atomic_load(&data->temperature));
    printf("Reactor Temp: %hd °C\n", atomic_load(&data->reactor_temperature));
    printf("IGBT Temp: %hd °C\n", atomic_load(&data->igbt_temperature));

    // 电能质量
    printf("--- Power Quality ---\n");
    printf("Power Factor: %.2f\n",
           atomic_load(&data->power_factor) / 10.0);   // 假设存储的是百分比*100

    // 三相电参数
    printf("--- Three-Phase System ---\n");
    for (int i = 0; i < 3; i++) {
        printf("Phase %c:", 'U' + i);
        printf("  Voltage: %hu V", atomic_load(&data->phase_voltage[i]));
        printf("  Current: %hd A\n", atomic_load(&data->phase_current[i]));
    }

    // 扩展参数
    printf("--- Extended Parameters ---");
    printf("Warning Code: 0x%04X\n", atomic_load(&data->warning));
    printf("Fault Code: 0x%08X\n", atomic_load(&data->fault));
    printf("SN: ");
    for (int i = 0; i < 16; i++) {
        printf("%04X ", atomic_load(&data->sn[i]));
    }
    printf("\n");

    // 运行模式
    printf("--- Control Parameters ---\n");
    printf("Power On/Off Status: %s\n",
           atomic_load(&data->except_power_on_off_status) ? "ON" : "OFF");
    printf("Active Power Command Mode: %u\n", atomic_load(&data->active_power_command_mode));
    printf("Max Active Power: %.1f 0.1kW\n",
           atomic_load(&data->max_active_power) / 10.0);   // 转换为kW单位

    // 光伏电流数组（示例打印前5个）
    printf("--- PV Current Array (First 5) ---\n");
    printf("[");
    for (int i = 0; i < PVPCS_PV_MAX_NUM; i++) {
        printf("%hu", atomic_load(&data->pv_current[i]));
        if (i < PVPCS_PV_MAX_NUM - 1) printf(", ");
        if (i >= 4) break;   // 仅打印前5个作示例
    }
    printf("]");
}
