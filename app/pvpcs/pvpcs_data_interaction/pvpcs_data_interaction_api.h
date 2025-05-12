//
// Created by 谷子 on 2025/4/7.
//

#ifndef PVPCS_DATA_INTERATION_API_H
#define PVPCS_DATA_INTERATION_API_H

#include <stdint.h>
#include <stdbool.h>

uint16_t pvpcs_work_mode_get(uint16_t device_id);
void pvpcs_work_mode_set(uint16_t device_id, uint16_t work_mode);

uint32_t pvpcs_daily_power_generation_get(uint16_t device_id);
void pvpcs_daily_power_generation_set(uint16_t device_id, uint32_t daily_power_generation);

uint32_t pvpcs_total_power_generation_get(uint16_t device_id);
void pvpcs_total_power_generation_set(uint16_t device_id, uint32_t total_power_generation);

uint32_t pvpcs_run_time_get(uint16_t device_id);
void pvpcs_run_time_set(uint16_t device_id, uint32_t run_time);

int32_t pvpcs_total_input_power_get(uint16_t device_id);
void pvpcs_total_input_power_set(uint16_t device_id, int32_t total_input_power);

int32_t pvpcs_total_active_power_get(uint16_t device_id);
void pvpcs_total_active_power_set(uint16_t device_id, int32_t total_active_power);

int32_t pvpcs_total_reactive_power_get(uint16_t device_id);
void pvpcs_total_reactive_power_set(uint16_t device_id, int32_t total_reactive_power);

int32_t pvpcs_total_apparent_power_get(uint16_t device_id);
void pvpcs_total_apparent_power_set(uint16_t device_id, int32_t total_apparent_power);

int16_t pvpcs_temperature_get(uint16_t device_id);
void pvpcs_temperature_set(uint16_t device_id, int16_t temperature);

int16_t pvpcs_power_factor_get(uint16_t device_id);
void pvpcs_power_factor_set(uint16_t device_id, int16_t power_factor);

uint16_t pvpcs_efficiency_get(uint16_t device_id);
void pvpcs_efficiency_set(uint16_t device_id, uint16_t efficiency);

int16_t pvpcs_reactor_temperature_get(uint16_t device_id);
void pvpcs_reactor_temperature_set(uint16_t device_id, int16_t reactor_temperature);

int16_t pvpcs_igbt_temperature_get(uint16_t device_id);
void pvpcs_igbt_temperature_set(uint16_t device_id, int16_t igbt_temperature);

int16_t pvpcs_pv_current_get(uint16_t device_id,uint16_t pv_id);
void pvpcs_pv_current_set(uint16_t device_id,uint16_t pv_id, int16_t current);

uint16_t pvpcs_pv_voltage_get(uint16_t device_id,uint16_t pv_id);
void pvpcs_pv_voltage_set(uint16_t device_id,uint16_t pv_id, uint16_t voltage);

uint16_t pvpcs_phase_a_voltage_get(uint16_t device_id);
void pvpcs_phase_a_voltage_set(uint16_t device_id, uint16_t voltage);

uint16_t pvpcs_phase_b_voltage_get(uint16_t device_id);
void pvpcs_phase_b_voltage_set(uint16_t device_id, uint16_t voltage);

uint16_t pvpcs_phase_c_voltage_get(uint16_t device_id);
void pvpcs_phase_c_voltage_set(uint16_t device_id, uint16_t current);

int16_t pvpcs_phase_a_current_get(uint16_t device_id);
void pvpcs_phase_a_current_set(uint16_t device_id, int16_t current);

int16_t pvpcs_phase_b_current_get(uint16_t device_id);
void pvpcs_phase_b_current_set(uint16_t device_id, int16_t current);

int16_t pvpcs_phase_c_current_get(uint16_t device_id);
void pvpcs_phase_c_current_set(uint16_t device_id, int16_t current);

uint16_t pvpcs_warning_get(uint16_t device_id);
void pvpcs_warning_set(uint16_t device_id, uint16_t warning);

uint32_t pvpcs_fault_get(uint16_t device_id);
void pvpcs_fault_set(uint16_t device_id, uint32_t fault);

void pvpcs_sn_get(uint16_t device_id, uint16_t* sn,uint8_t len);
void pvpcs_sn_set(uint16_t device_id, uint16_t* sn,uint8_t len);

bool pvpcs_except_power_on_off_status_get(uint16_t device_id);
void pvpcs_except_power_on_off_status_set(uint16_t device_id, bool on);

uint32_t pvpcs_except_active_power_get(uint16_t device_id);
void pvpcs_except_active_power_set(uint16_t device_id, uint32_t active_power);

uint8_t pvpcs_active_power_command_mode_get(uint16_t device_id);
void pvpcs_active_power_command_mode_set(uint16_t device_id, uint8_t mode);

uint32_t pvpcs_max_active_power_get(uint16_t device_id);
void pvpcs_max_active_power_set(uint16_t device_id, uint32_t max_active_power);

bool pvpcs_connect_state_get(uint16_t device_id);
void pvpcs_connect_state_set(uint16_t device_id, bool connect_state);

uint32_t pvpcs_connect_status_get(void);
void pvpcs_connect_status_set(uint32_t connect_status);

uint32_t pvpcs_multi_daily_power_generation_get(void);
void pvpcs_multi_daily_power_generation_set(uint32_t daily_power_generation);

uint32_t pvpcs_multi_total_power_generation_get(void);
void pvpcs_multi_total_power_generation_set(uint32_t total_power_generation);

uint16_t pvpcs_multi_active_power_get(void);
void pvpcs_multi_active_power_set(uint16_t total_active_power);

uint32_t pvpcs_multi_max_active_power_get(void);
void pvpcs_multi_max_active_power_set(uint32_t total_max_active_power);

void pvpcs_loss_update(uint16_t device_id);
void pvpcs_connect_state_update(uint16_t device_id);

bool pvpcs_single_if_availability(uint16_t device_id);

uint16_t pvpcs_availability_num_get(void);//获取能够正常运行的光伏设备数量
void pvpcs_active_power_distribution(uint32_t except_active_power);//光伏总功率均分下发

uint32_t pvpcs_total_except_active_power_get(void);
void pvpcs_total_except_active_power_set(uint32_t except_active_power);

void print_pvpcs_single_data(uint16_t device_id);

#endif //PVPCS_DATA_INTERATION_API_H
