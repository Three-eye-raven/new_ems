//
// Created by three on 2025/4/25.
//
#include "pvpcs_hw_110kw_external_api.h"
#include "pvpcs_data_interaction_api.h"

#include <string.h>

void pvpcs_hw_110kw_pmax_read_back_data_process(const uint16_t deviceId, void* pRawData,
                                                uint8_t dataSize)
{
        const uint32_t maxActivePower = *(uint32_t*)pRawData / 100;   // 0.1kw
        pvpcs_max_active_power_set(deviceId, maxActivePower);
}

void pvpcs_hw_110kw_status_read_back_data_process(const uint16_t deviceId, void* pRawData,
                                                  uint8_t dataSize)
{
        const uint16_t status = *(uint16_t*)pRawData;
        if (status == 0x0000 || status == 0x0001 || status == 0x0002 || status == 0x0003 ||
            status == 0x0100 || status == 0x0301 || status == 0x0304 || status == 0x0307 ||
            status == 0x0500 || status == 0x0501 || status == 0x0600 || status == 0x0700) {
                // 设备待机
                pvpcs_work_mode_set(deviceId, 0);
        }
        else if (status == 0x0200 || status == 0x0201 || status == 0x0202) {
                // 设备运行
                pvpcs_work_mode_set(deviceId, 1);
        }
        else {
                // 设备故障
                pvpcs_work_mode_set(deviceId, 2);
        }
}

void pvpcs_hw_110kw_input_power_read_back_data_process(const uint16_t deviceId, void* pRawData,
                                                       uint8_t dataSize)
{
        const int32_t power = *(int32_t*)pRawData / 100;
        pvpcs_total_input_power_set(deviceId, power);
}

void pvpcs_hw_110kw_active_power_read_back_data_process(const uint16_t deviceId, void* pRawData,
                                                        uint8_t dataSize)
{
        const int32_t activePower = *(int32_t*)pRawData / 100;
        pvpcs_total_active_power_set(deviceId, activePower);
}

void pvpcs_hw_110kw_power_generation_read_back_data_process(const uint16_t deviceId, void* pRawData,
                                                            uint8_t dataSize)
{
        const uint32_t daily = *(uint32_t*)pRawData / 10;
        const uint32_t total = *(uint32_t*)(pRawData + sizeof(uint32_t)) / 10;
        pvpcs_daily_power_generation_set(deviceId, daily);
        pvpcs_total_power_generation_set(deviceId, total);
}

void pvpcs_hw_110kw_active_power_info_read_back_data_process(const uint16_t deviceId,
                                                             void* pRawData, uint8_t dataSize)
{
        const uint16_t mode = *(uint16_t*)pRawData;
        pvpcs_active_power_command_mode_set(deviceId, mode);
}

bool pvpcs_hw_110kw_active_power_fix_value_set_write_data_get(const uint16_t deviceId,
                                                              void*          pRawData)
{
        bool          ret  = false;
        const uint8_t mode = pvpcs_active_power_command_mode_get(deviceId);
        if (1 == mode && NULL != pRawData) {
                ret                              = true;
                const uint32_t exceptActivePower = pvpcs_except_active_power_get(deviceId);
                const uint32_t maxActivePower    = pvpcs_max_active_power_get(deviceId);
                uint16_t       val               = maxActivePower;
                if (maxActivePower > exceptActivePower) {
                        val = (uint16_t)(maxActivePower - exceptActivePower);
                }
                memcpy(pRawData, &val, sizeof(val));
        }
        return ret;
}
bool pvpcs_hw_110kw_active_power_percentage_set_write_data_get(const uint16_t deviceId,
                                                               void*          pRawData)
{
        bool          ret  = false;
        const uint8_t mode = pvpcs_active_power_command_mode_get(deviceId);
        if (0 == mode) {
                ret                              = true;
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