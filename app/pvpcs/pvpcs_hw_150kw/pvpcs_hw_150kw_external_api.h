//
// Created by three on 2025/4/27.
//

#ifndef PVPCS_HW_150KW_EXTERNAL_API_H
#define PVPCS_HW_150KW_EXTERNAL_API_H

#include <stdbool.h>
#include <stdint.h>

void pvpcs_hw_150kw_pmax_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                uint8_t dataSize);
void pvpcs_hw_150kw_dc_input_power_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                          uint8_t data_Size);
void pvpcs_hw_150kw_active_power_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                        uint8_t data_Size);
void pvpcs_hw_150kw_device_status_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                         uint8_t data_Size);
void pvpcs_hw_150kw_total_generation_power_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                                  uint8_t data_Size);
void pvpcs_hw_150kw_daily_generation_power_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                                  uint8_t data_Size);

bool pvpcs_hw_150kw_active_power_percentage_set_write_data_get(uint16_t deviceId, void* pRawData);

#endif   // PVPCS_HW_150KW_EXTERNAL_API_H
