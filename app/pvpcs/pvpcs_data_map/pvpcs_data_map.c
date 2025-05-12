//
// Created by 谷子 on 2025/4/9.
//

#include "pvpcs_data_map.h"
#include "pvpcs_data_interaction_api.h"

#include <string.h>

void pvpcs_single_data_map(uint16_t device_id,uint16_t *pDst)
{
    uint16_t u16temp =0;
    int16_t s16temp =0;
    uint32_t u32temp =0;
    int32_t s32temp =0;
    uint16_t addr_offset = device_id * PVPCS_MAP_ADDR_OFFSET;
    pDst[PVPCS_WORK_MODE_MAP_ADDR + addr_offset] = pvpcs_work_mode_get(device_id);
    u32temp = pvpcs_daily_power_generation_get(device_id);
    pDst[PVPCS_DAILY_POWER_GENERATION_H_MAP_ADDR + addr_offset] = u32temp >> 16;
    pDst[PVPCS_DAILY_POWER_GENERATION_L_MAP_ADDR + addr_offset] = u32temp & 0x0000FFFF;
    u32temp = pvpcs_total_power_generation_get(device_id);
    pDst[PVPCS_TOTAL_POWER_GENERATION_H_MAP_ADDR + addr_offset] = u32temp >> 16;
    pDst[PVPCS_TOTAL_POWER_GENERATION_L_MAP_ADDR + addr_offset] = u32temp & 0x0000FFFF;
    u32temp = pvpcs_run_time_get(device_id);
    pDst[PVPCS_RUN_TIME_H_MAP_ADDR + addr_offset] = u32temp >> 16;
    pDst[PVPCS_RUN_TIME_L_MAP_ADDR + addr_offset] = u32temp & 0x0000FFFF;
    s32temp = pvpcs_total_input_power_get(device_id);
    memcpy(&u32temp,&s32temp,sizeof(int32_t));
    pDst[PVPCS_TOTAL_INPUT_POWER_H_MAP_ADDR + addr_offset] = u32temp >> 16;
    pDst[PVPCS_TOTAL_INPUT_POWER_L_MAP_ADDR + addr_offset] = u32temp & 0x0000FFFF;
    s32temp = pvpcs_total_active_power_get(device_id);
    memcpy(&u32temp,&s32temp,sizeof(int32_t));
    pDst[PVPCS_TOTAL_ACTIVE_POWER_H_MAP_ADDR + addr_offset] = u32temp >> 16;
    pDst[PVPCS_TOTAL_ACTIVE_POWER_L_MAP_ADDR + addr_offset] = u32temp & 0x0000FFFF;
    s32temp = pvpcs_total_reactive_power_get(device_id);
    memcpy(&u32temp,&s32temp,sizeof(int32_t));
    pDst[PVPCS_TOTAL_REACTIVE_POWER_H_MAP_ADDR + addr_offset] = u32temp >> 16;
    pDst[PVPCS_TOTAL_REACTIVE_POWER_L_MAP_ADDR + addr_offset] = u32temp & 0x0000FFFF;
    s32temp = pvpcs_total_apparent_power_get(device_id);
    memcpy(&u32temp,&s32temp,sizeof(int32_t));
    pDst[PVPCS_TOTAL_APPARENT_POWER_H_MAP_ADDR + addr_offset] = u32temp >> 16;
    pDst[PVPCS_TOTAL_APPARENT_POWER_L_MAP_ADDR + addr_offset] = u32temp & 0x0000FFFF;
    s16temp = pvpcs_temperature_get(device_id);
    memcpy(&u16temp,&s16temp,sizeof(int16_t));
    pDst[PVPCS_TEMPERATURE_MAP_ADDR + addr_offset] = u16temp;
    s16temp = pvpcs_power_factor_get(device_id);
    memcpy(&u16temp,&s16temp,sizeof(int16_t));
    pDst[PVPCS_POWER_FACTOR_MAP_ADDR + addr_offset] = u16temp;
    pDst[PVPCS_EFFICIENCY_MAP_ADDR + addr_offset] = pvpcs_efficiency_get(device_id);
    s16temp = pvpcs_reactor_temperature_get(device_id);
    memcpy(&u16temp,&s16temp,sizeof(int16_t));
    pDst[PVPCS_REACTOR_TEMPERATURE_MAP_ADDR + addr_offset] = u16temp;
    s16temp = pvpcs_igbt_temperature_get(device_id);
    memcpy(&u16temp,&s16temp,sizeof(int16_t));
    pDst[PVPCS_IGBT_TEMPERATURE_MAP_ADDR + addr_offset] = u16temp;
    for(uint8_t idx = 0 ;idx < 32;idx++)
    {
        s16temp = pvpcs_pv_current_get(device_id,idx);
        memcpy(&u16temp,&s16temp,sizeof(int16_t));
        pDst[PVPCS_IDC_BASE_MAP_ADDR + idx*2 + addr_offset] = u16temp;
        pDst[PVPCS_VDC_BASE_MAP_ADDR + idx*2 + addr_offset] = pvpcs_pv_voltage_get(device_id,idx);
    }
    pDst[PVPCS_PHASE_A_VOLTAGE_MAP_ADDR + addr_offset] = pvpcs_phase_a_voltage_get(device_id);
    pDst[PVPCS_PHASE_B_VOLTAGE_MAP_ADDR + addr_offset] = pvpcs_phase_b_voltage_get(device_id);
    pDst[PVPCS_PHASE_C_VOLTAGE_MAP_ADDR + addr_offset] = pvpcs_phase_c_voltage_get(device_id);

    s16temp = pvpcs_phase_a_current_get(device_id);
    memcpy(&u16temp,&s16temp,sizeof(int16_t));
    pDst[PVPCS_PHASE_A_CURRENT_MAP_ADDR + addr_offset] = u16temp;
    s16temp = pvpcs_phase_b_current_get(device_id);
    memcpy(&u16temp,&s16temp,sizeof(int16_t));
    pDst[PVPCS_PHASE_B_CURRENT_MAP_ADDR + addr_offset] = u16temp;
    s16temp = pvpcs_phase_c_current_get(device_id);
    memcpy(&u16temp,&s16temp,sizeof(int16_t));
    pDst[PVPCS_PHASE_C_CURRENT_MAP_ADDR + addr_offset] = u16temp;

    pDst[PVPCS_WARNING_MAP_ADDR + addr_offset] = pvpcs_warning_get(device_id);
    u32temp = pvpcs_fault_get(device_id);
    pDst[PVPCS_FAULT_H_MAP_ADDR + addr_offset] = u32temp >> 16;
    pDst[PVPCS_FAULT_L_MAP_ADDR + addr_offset] = u32temp & 0x0000FFFF;
    pvpcs_sn_get(device_id,&pDst[PVPCS_SN_MAP_ADDR + addr_offset],8);
}

void pvpcs_multi_data_map(uint16_t *pDst)
{
    uint32_t u32temp = 0;
    u32temp = pvpcs_connect_status_get();
    pDst[PVPCS_MULTI_CONNECT_STATUS_H_MAP_ADDR] = u32temp >> 16;
    pDst[PVPCS_MULTI_CONNECT_STATUS_L_MAP_ADDR] = u32temp & 0x0000FFFF;
    u32temp = pvpcs_multi_daily_power_generation_get();
    pDst[PVPCS_MULTI_DAILY_POWER_GENERATION_H_MAP_ADDR] = u32temp >> 16;
    pDst[PVPCS_MULTI_DAILY_POWER_GENERATION_L_MAP_ADDR] = u32temp & 0x0000FFFF;
    u32temp = pvpcs_multi_total_power_generation_get();
    pDst[PVPCS_MULTI_TOTAL_POWER_GENERATION_H_MAP_ADDR] = u32temp >> 16;
    pDst[PVPCS_MULTI_TOTAL_POWER_GENERATION_L_MAP_ADDR] = u32temp & 0x0000FFFF;
    pDst[PVPCS_MULTI_TOTAL_ACTIVE_POWER_MAP_ADDR] = pvpcs_multi_active_power_get();
}