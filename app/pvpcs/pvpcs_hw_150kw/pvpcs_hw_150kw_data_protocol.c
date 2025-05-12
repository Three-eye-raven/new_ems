//
// Created by three on 2025/4/27.
//

#include "pvpcs_hw_150kw_data_protocol.h"
#include "pvpcs_hw_150kw_external_api.h"
#include "pvpcs_hw_150kw_link_api.h"

#include "modbus_master_struct.h"

static ModbusMasterReadableSyncInfo_T readableInfos[PVPCS_HW_150KW_READ_DATA_INFOS_NUM] = {
        [R_PVPCS_HW_150KW_PMAX] =
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
                                                pvpcs_hw_150kw_pmax_read_back_data_process,
                                },
                },
        [R_PVPCS_HW_150KW_DC_INPUT_POWER] =
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
                                                pvpcs_hw_150kw_dc_input_power_read_back_data_process,
                                },
                },
        [R_PVPCS_HW_150KW_ACTIVE_POWER] =
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
                                                pvpcs_hw_150kw_active_power_read_back_data_process,
                                },
                },
        [R_PVPCS_HW_150KW_DEVICE_STATUS] =
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
                                                pvpcs_hw_150kw_device_status_read_back_data_process,
                                },
                },
        [R_PVPCS_HW_150KW_TOTAL_GENERATION_POWER] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 32106,
                                        .length    = 2,
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
                                                pvpcs_hw_150kw_total_generation_power_read_back_data_process,
                                },
                },
        [R_PVPCS_HW_150KW_DAILY_GENERATION_POWER] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 32114,
                                        .length    = 2,
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
                                                pvpcs_hw_150kw_daily_generation_power_read_back_data_process,
                                },
                },
};

static ModbusMasterWritableSyncInfo_T writableInfos[PVPCS_HW_150KW_WRITE_DATA_INFOS_NUM] = {
        [W_PVPCS_HW_150KW_ACTIVE_POWER_PRECENTAGE_SET] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 40199,
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
                                                pvpcs_hw_150kw_active_power_percentage_set_write_data_get,
                                },
                },
};


ModbusMasterDataProtocol_T pvpcsHw150kwDataProtocol = {
        .protocolsName        = "pvpcs hw 150kw",
        .pReadableSyncInfos   = readableInfos,
        .readableSyncInfosNum = PVPCS_HW_150KW_READ_DATA_INFOS_NUM,
        .pWritableSyncInfos   = writableInfos,
        .writableSyncInfosNum = PVPCS_HW_150KW_WRITE_DATA_INFOS_NUM,
        .time =
                {
                .frameInterval    = 10,
                .retryInterval    = 10 * 1000,
                .delayCardinality = 1,
                .timeoutThreshold = 5 * 1000,
        },
.linkApi =
        {
                .lose    = pvpcs_hw_150kw_link_lose,
                .online  = NULL,
                .connect = pvpcs_hw_150kw_link_connect,
        },
};
