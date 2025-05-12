//
// Created by three on 2025/4/25.
//

#ifndef PVPCS_SOFAR_50KW_EXTERNAL_H
#define PVPCS_SOFAR_50KW_EXTERNAL_H

#include <stdbool.h>
#include <stdint.h>

void pvpcs_sofar_50kw_status_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                    uint8_t dataSize);
void pvpcs_sofar_50kw_active_power_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                          uint8_t dataSize);
void pvpcs_sofar_50kw_input_power_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                         uint8_t dataSize);
void pvpcs_sofar_50kw_power_generation_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                              uint8_t dataSize);
void pvpcs_sofar_50kw_pmax_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                  uint8_t dataSize);

bool pvpcs_sofar_50kw_active_power_precentage_set_write_data_get(uint16_t deviceId, void* pRawData);


#endif //PVPCS_SOFAR_50KW_EXTERNAL_H
