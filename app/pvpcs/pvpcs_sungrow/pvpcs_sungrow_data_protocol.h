//
// Created by three on 2025/4/27.
//

#ifndef PVPCS_SUNGROW_DATA_PROTOCOL_H
#define PVPCS_SUNGROW_DATA_PROTOCOL_H

#define PVPCS_SUNGROW_READ_DATA_INFOS_NUM 6
#define PVPCS_SUNGROW_WRITE_DATA_INFOS_NUM 2

typedef enum
{
        R_PVPVS_SUNGROW_PMAX,//5000,0.1kw,u16
        R_PVPCS_SUNGROW_DAILY_GENERATION_POWER,//5002,0.1kWh,u16
        R_PVPCS_SUNGROW_TOTAL_GENERATION_POWER,//5003,kWh,u32
        R_PVPCS_SUNGROW_DC_INPUT_POWER,//5016,w,u32
        R_PVPCS_SUNGROW_ACTIVE_POWER,//5030,w,s32
        R_PVPCS_SUNGROW_DEVICE_STATUS,//5037,u16
}PVPCS_SUNGROW_READ_DATA_NAME_E;

typedef enum
{
        W_PVPCS_SUNGROW_POWER_SET_ENABLE,//5006,u16,0xAA
        W_PVPCS_SUNGROW_ACTIVE_POWER_PRECENTAGE_SET,//5007,u16,0.1%
}PVPCS_SUNGROW_WRITE_DATA_NAME_E;

#endif //PVPCS_SUNGROW_DATA_PROTOCOL_H
