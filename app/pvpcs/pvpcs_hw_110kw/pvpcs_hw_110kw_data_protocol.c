//
// Created by three on 2025/4/25.
//

#include "pvpcs_hw_110kw_data_protocol.h"
#include "modbus_master_struct.h"
#include "pvpcs_hw_110kw_external_api.h"
#include "pvpcs_hw_110kw_link_api.h"

static ModbusMasterReadableSyncInfo_T readableInfos[PVPCS_HW_110KW_READ_DATA_INFOS_NUM] = {
        [R_PVPCS_HW_110KW_PMAX] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 30075,
                                        .length    = 2,
                                        .regType   = MODBUS_MASTER_HOLDING_REG,
                                        .filpStep  = MODBUS_MASTER_FILP_2,
                                },
                        .Schedule =
                                {
                                        .interval    = 0,
                                        .firstRecord = {false},
                                        .lastTime    = {0},
                                },
                        .ExternalApi =
                                {
                                        .read_back_data_process =
                                                pvpcs_hw_110kw_pmax_read_back_data_process,
                                },
                },
        [R_PVPCS_HW_110KW_STATUS] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 32089,
                                        .length    = 1,
                                        .regType   = MODBUS_MASTER_HOLDING_REG,
                                        .filpStep  = MODBUS_MASTER_FILP_0,
                                },
                        .Schedule =
                                {
                                        .interval    = 50,
                                        .firstRecord = {false},
                                        .lastTime    = {0},
                                },
                        .ExternalApi =
                                {
                                        .read_back_data_process =
                                                pvpcs_hw_110kw_status_read_back_data_process,
                                },
                },
        [R_PVPCS_HW_110KW_INPUT_POWER] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 32064,
                                        .length    = 2,
                                        .regType   = MODBUS_MASTER_HOLDING_REG,
                                        .filpStep  = MODBUS_MASTER_FILP_2,
                                },
                        .Schedule =
                                {
                                        .interval    = 50,
                                        .firstRecord = {false},
                                        .lastTime    = {0},
                                },
                        .ExternalApi =
                                {
                                        .read_back_data_process =
                                                pvpcs_hw_110kw_input_power_read_back_data_process,
                                },
                },
        [R_PVPCS_HW_110KW_ACTIVE_POWER] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 32080,
                                        .length    = 2,
                                        .regType   = MODBUS_MASTER_HOLDING_REG,
                                        .filpStep  = MODBUS_MASTER_FILP_2,
                                },
                        .Schedule =
                                {
                                        .interval    = 50,
                                        .firstRecord = {false},
                                        .lastTime    = {0},
                                },
                        .ExternalApi =
                                {
                                        .read_back_data_process =
                                                pvpcs_hw_110kw_active_power_read_back_data_process,
                                },
                },
        [R_PVPCS_HW_110KW_POWER_GENERATION] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 32106,
                                        .length    = 4,
                                        .regType   = MODBUS_MASTER_HOLDING_REG,
                                        .filpStep  = MODBUS_MASTER_FILP_2,
                                },
                        .Schedule =
                                {
                                        .interval    = 150 * 1000,
                                        .firstRecord = {false},
                                        .lastTime    = {0},
                                },
                        .ExternalApi =
                                {
                                        .read_back_data_process =
                                                pvpcs_hw_110kw_power_generation_read_back_data_process,
                                },
                },
        [R_PVPCS_HW_110KW_ACTIVE_POWER_INFO] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 35300,
                                        .length    = 4,
                                        .regType   = MODBUS_MASTER_HOLDING_REG,
                                        .filpStep  = MODBUS_MASTER_FILP_0,
                                },
                        .Schedule =
                                {
                                        .interval    = 50,
                                        .firstRecord = {false},
                                        .lastTime    = {0},
                                },
                        .ExternalApi =
                                {
                                        .read_back_data_process =
                                                pvpcs_hw_110kw_active_power_info_read_back_data_process,
                                },
                },
};

static ModbusMasterWritableSyncInfo_T writableInfos[PVPCS_HW_110KW_WRITE_DATA_INFOS_NUM] = {
        [W_PVPCS_HW_110KW_ACTIVE_POWER_FIX_VALUE_SET] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 40120,
                                        .length    = 1,
                                        .regType   = MODBUS_MASTER_HOLDING_REG,
                                        .filpStep  = MODBUS_MASTER_FILP_0,
                                },
                        .Schedule =
                                {
                                        .interval = 50,
                                        .lastTime = {0},
                                },
                        .ExternalApi =
                                {
                                        .write_data_get =
                                                pvpcs_hw_110kw_active_power_fix_value_set_write_data_get,
                                },
                },
        [W_PVPCS_HW_110KW_ACTIVE_POWER_PERCENTAGE_SET] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 40125,
                                        .length    = 1,
                                        .regType   = MODBUS_MASTER_HOLDING_REG,
                                        .filpStep  = MODBUS_MASTER_FILP_0,
                                },
                        .Schedule =
                                {
                                        .interval = 50,
                                        .lastTime = {0},
                                },
                        .ExternalApi =
                                {
                                        .write_data_get =
                                                pvpcs_hw_110kw_active_power_percentage_set_write_data_get,
                                },
                },
};

ModbusMasterDataProtocol_T pvpcsHw110kwDataProtocol = {
        .protocolsName        = "pvpcs hw 110kw",
        .pReadableSyncInfos   = readableInfos,
        .readableSyncInfosNum = PVPCS_HW_110KW_READ_DATA_INFOS_NUM,
        .pWritableSyncInfos   = writableInfos,
        .writableSyncInfosNum = PVPCS_HW_110KW_WRITE_DATA_INFOS_NUM,
        .time =
                {
                        .frameInterval    = 10,
                        .retryInterval    = 10 * 1000,
                        .delayCardinality = 1,
                        .timeoutThreshold = 5 * 1000,
                },
        .linkApi =
                {
                        .lose    = pvpcs_hw_110kw_link_lose,
                        .online  = NULL,
                        .connect = pvpcs_hw_110kw_link_connect,
                },
};
