//
// Created by three on 2025/4/25.
//

#ifndef PVPCS_HW_110KW_EXTERNAL_API_H
#define PVPCS_HW_110KW_EXTERNAL_API_H

#include <stdbool.h>
#include <stdint.h>

void pvpcs_hw_110kw_pmax_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                uint8_t dataSize);
void pvpcs_hw_110kw_status_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                  uint8_t dataSize);
void pvpcs_hw_110kw_input_power_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                       uint8_t dataSize);
void pvpcs_hw_110kw_active_power_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                        uint8_t dataSize);
void pvpcs_hw_110kw_power_generation_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                            uint8_t dataSize);
void pvpcs_hw_110kw_active_power_info_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                             uint8_t dataSize);

bool pvpcs_hw_110kw_active_power_fix_value_set_write_data_get(uint16_t deviceId, void* pRawData);
bool pvpcs_hw_110kw_active_power_percentage_set_write_data_get(uint16_t deviceId, void* pRawData);


#endif   // PVPCS_HW_110KW_EXTERNAL_API_H
