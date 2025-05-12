//
// Created by three on 2025/4/25.
//

#include "pvpcs_sofar_50kw_external_api.h"
#include "pvpcs_data_interaction_api.h"

#include <string.h>

void pvpcs_sofar_50kw_status_read_back_data_process(const uint16_t deviceId, void* pRawData,
                                                    uint8_t dataSize)
{
        const uint16_t status = *(uint16_t*)pRawData;
        if (2 == status) {
                // 设备运行
                pvpcs_work_mode_set(deviceId, 1);
        }
        else {
                //故障状态
                pvpcs_work_mode_set(deviceId, 2);
        }
}

void pvpcs_sofar_50kw_active_power_read_back_data_process(const uint16_t deviceId, void* pRawData,
                                                          uint8_t dataSize)
{
        const int32_t activePower = *(int32_t*)pRawData / 10;
        pvpcs_total_active_power_set(deviceId, activePower);}

void pvpcs_sofar_50kw_input_power_read_back_data_process(const uint16_t deviceId, void* pRawData,
                                                         uint8_t dataSize)
{
        const int32_t power = *(int32_t*)pRawData;
        pvpcs_total_input_power_set(deviceId, power);
}

void pvpcs_sofar_50kw_power_generation_read_back_data_process(const uint16_t deviceId, void* pRawData,
                                                              uint8_t dataSize)
{
        const uint32_t daily = *(uint32_t*)pRawData / 100;
        const uint32_t total = *(uint32_t*)(pRawData + sizeof(uint32_t)) / 10;
        pvpcs_daily_power_generation_set(deviceId, daily);
        pvpcs_total_power_generation_set(deviceId, total);
}

void pvpcs_sofar_50kw_pmax_read_back_data_process(const uint16_t deviceId, void* pRawData,
                                                  uint8_t dataSize)
{
        const uint32_t maxActivePower = *(uint32_t*)pRawData;   // 0.1kw
        pvpcs_max_active_power_set(deviceId, maxActivePower);
}

bool pvpcs_sofar_50kw_active_power_precentage_set_write_data_get(const uint16_t deviceId, void* pRawData)
{
        bool ret = true;
        const uint32_t exceptActivePower = pvpcs_except_active_power_get(deviceId);
        const uint32_t maxActivePower    = pvpcs_max_active_power_get(deviceId);
        uint16_t       val               = 1000;
        if (maxActivePower > exceptActivePower) {
                val = (uint16_t)(1000 * (maxActivePower - exceptActivePower) /
                                 maxActivePower);
        }
                memcpy(pRawData, &val, sizeof(val));
        return ret;
}

