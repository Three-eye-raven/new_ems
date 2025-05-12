//
// Created by three on 2025/4/27.
//

#include "pvpcs_sungrow_external_api.h"
#include "pvpcs_data_interaction_api.h"

#include <string.h>

void pvpcs_sungrow_pmax_read_back_data_process(const uint16_t deviceId, void* pRawData,
                                               uint8_t dataSize)
{
        const uint16_t pmax = *(uint16_t*)pRawData;
        pvpcs_max_active_power_set(deviceId, pmax);
}

void pvpcs_sungrow_daily_generation_power_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                                 uint8_t data_Size)
{
        const uint16_t daily = *(uint16_t*)pRawData /10;
        pvpcs_daily_power_generation_set(deviceId,daily);
}

void pvpcs_sungrow_total_generation_power_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                                 uint8_t data_Size)
{
        const uint32_t total = *(uint32_t*)pRawData /10;
        pvpcs_total_power_generation_set(deviceId,total);
}

void pvpcs_sungrow_dc_input_power_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                         uint8_t data_Size)
{
        const uint32_t dc_input = *(uint32_t*)pRawData /100;
        pvpcs_total_input_power_set(deviceId, dc_input);
}

void pvpcs_sungrow_active_power_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                       uint8_t data_Size)
{
        const int32_t activePower = *(int32_t*)pRawData / 100;
        pvpcs_total_active_power_set(deviceId, activePower);
}

void pvpcs_sungrow_device_status_read_back_data_process(uint16_t deviceId, void* pRawData,
                                                        uint8_t data_Size)
{
        const uint16_t work_status = *((uint16_t*)pRawData);
        if (work_status == 0x0 || work_status == 0x9100 || work_status == 0x8100 ||
            work_status == 0x8200) {
                // 设备运行
                pvpcs_work_mode_set(deviceId, 1);
            }
        else if (work_status == 0x8000 || work_status == 0x1400 || work_status == 0x1200 ||
                 work_status == 0x1600) {
                // 设备待机
                pvpcs_work_mode_set(deviceId, 0);
                 }
        else {
                // 设备故障
                pvpcs_work_mode_set(deviceId, 2);
        }
}

bool pvpcs_sungrow_power_set_enable_write_data_get(uint16_t deviceId, void* pRawData)
{
        bool ret = false;
        if (NULL != pRawData) {
                ret = true;
                const uint16_t u16temp = 0xAA;
                memcpy(pRawData, &u16temp, sizeof(uint16_t));
        }
        return ret;
}

bool pvpcs_sungrow_active_power_percentage_set_write_data_get(uint16_t deviceId, void* pRawData)
{
        bool ret = false;
        if (NULL != pRawData) {
                ret = true;
                const uint32_t exceptActivePower = pvpcs_except_active_power_get(deviceId);
                const uint32_t maxActivePower    = pvpcs_max_active_power_get(deviceId);
                uint16_t       val               = 1000;
                if (maxActivePower > exceptActivePower) {
                        val = (uint16_t)(1000 * (maxActivePower - exceptActivePower) /
                                         maxActivePower);
                }
                memcpy(pRawData, &val, sizeof(val));
        }
        return ret;
}
