//
// Created by three on 2025/4/27.
//

#include "pvpcs_sungrow_data_protocol.h"
#include "pvpcs_sungrow_external_api.h"
#include "pvpcs_sungrow_link_api.h"

#include "modbus_master_struct.h"

static ModbusMasterReadableSyncInfo_T readableInfos[PVPCS_SUNGROW_READ_DATA_INFOS_NUM] = {
        [R_PVPVS_SUNGROW_PMAX] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 5000,
                                        .length    = 1,
                                        .regType   = MODBUS_MASTER_INPUT_REG,
                                        .filpStep  = MODBUS_MASTER_FILP_0,
                                },
                        .Schedule =
                                {
                                        .interval    = 0,
                                        .firstRecord = {false},
                                        .lastTime    = {0},
                                },
                        .ExternalApi =
                                {
                                        .read_back_data_process = pvpcs_sungrow_pmax_read_back_data_process,
                                },
                },
        [R_PVPCS_SUNGROW_DAILY_GENERATION_POWER] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 5002,
                                        .length    = 1,
                                        .regType   = MODBUS_MASTER_INPUT_REG,
                                        .filpStep  = MODBUS_MASTER_FILP_0,
                                },
                        .Schedule =
                                {
                                        .interval    = 150 * 1000,
                                        .firstRecord = {false},
                                        .lastTime    = {0},
                                },
                        .ExternalApi =
                                {
                                        .read_back_data_process = pvpcs_sungrow_daily_generation_power_read_back_data_process,
                                },
                },
        [R_PVPCS_SUNGROW_TOTAL_GENERATION_POWER] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 5003,
                                        .length    = 2,
                                        .regType   = MODBUS_MASTER_INPUT_REG,
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
                                        .read_back_data_process = pvpcs_sungrow_total_generation_power_read_back_data_process,
                                },
                },
        [R_PVPCS_SUNGROW_DC_INPUT_POWER] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 5016,
                                        .length    = 2,
                                        .regType   = MODBUS_MASTER_INPUT_REG,
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
                                        .read_back_data_process = pvpcs_sungrow_dc_input_power_read_back_data_process,
                                },
                },
        [R_PVPCS_SUNGROW_ACTIVE_POWER] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 5030,
                                        .length    = 2,
                                        .regType   = MODBUS_MASTER_INPUT_REG,
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
                                        .read_back_data_process = pvpcs_sungrow_active_power_read_back_data_process,
                                },
                },
        [R_PVPCS_SUNGROW_DEVICE_STATUS] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 5037,
                                        .length    = 1,
                                        .regType   = MODBUS_MASTER_INPUT_REG,
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
                                        .read_back_data_process = pvpcs_sungrow_device_status_read_back_data_process,
                                },
                },
};

static ModbusMasterWritableSyncInfo_T writableInfos[PVPCS_SUNGROW_WRITE_DATA_INFOS_NUM] = {
        [W_PVPCS_SUNGROW_POWER_SET_ENABLE] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 5006,
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
                                        .write_data_get = pvpcs_sungrow_power_set_enable_write_data_get,
                                },
                },
        [W_PVPCS_SUNGROW_ACTIVE_POWER_PRECENTAGE_SET] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 5007,
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
                                        .write_data_get = pvpcs_sungrow_active_power_percentage_set_write_data_get,
                                },
                },
};

ModbusMasterDataProtocol_T pvpcsSungrowDataProtocol = {
        .protocolsName        = "pvpcs sungrow",
        .pReadableSyncInfos   = readableInfos,
        .readableSyncInfosNum = PVPCS_SUNGROW_READ_DATA_INFOS_NUM,
        .pWritableSyncInfos   = writableInfos,
        .writableSyncInfosNum = PVPCS_SUNGROW_WRITE_DATA_INFOS_NUM,
        .time =
                {
                        .frameInterval    = 10,
                        .retryInterval    = 10 * 1000,
                        .delayCardinality = 1,
                        .timeoutThreshold = 5 * 1000,
                },
        .linkApi =
                {
                        .lose    = pvpcs_sungrow_link_lose,
                        .online  = NULL,
                        .connect = pvpcs_sungrow_link_connect,
                },
};

