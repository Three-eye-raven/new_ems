//
// Created by three on 2025/4/25.
//

#include "pvpcs_sofar_50kw_data_protocol.h"
#include "modbus_master_struct.h"
#include "pvpcs_sofar_50kw_external_api.h"
#include "pvpcs_sofar_50kw_link_api.h"

static ModbusMasterReadableSyncInfo_T readableInfos[R_PVPCS_SOFAR_50KW_READ_DATA_INFOS_NUM] = {
        [R_PVPCS_SOFAR_50KW_STATUS] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 0x0404,
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
                                        .read_back_data_process = pvpcs_sofar_50kw_status_read_back_data_process,
                                },
                },
        [R_PVPCS_SOFAR_50KW_ACTIVE_POWER] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 0x0485,
                                        .length    = 1,
                                        .regType   = MODBUS_MASTER_HOLDING_REG,
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
                                        .read_back_data_process = pvpcs_sofar_50kw_active_power_read_back_data_process,
                                },
                },
        [R_PVPCS_SOFAR_50KW_INPUT_POWER] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 0x05c4,
                                        .length    = 1,
                                        .regType   = MODBUS_MASTER_HOLDING_REG,
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
                                        .read_back_data_process = pvpcs_sofar_50kw_input_power_read_back_data_process,
                                },
                },
        [R_PVPCS_SOFAR_50KW_POWER_GENERATION] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 0x0684,
                                        .length    = 4,
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
                                        .read_back_data_process = pvpcs_sofar_50kw_power_generation_read_back_data_process,
                                },
                },
        [R_PVPCS_SOFAR_50KW_PMAX] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 0x06ED,
                                        .length    = 1,
                                        .regType   = MODBUS_MASTER_HOLDING_REG,
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
                                        .read_back_data_process = pvpcs_sofar_50kw_pmax_read_back_data_process,
                                },
                },
};

static ModbusMasterWritableSyncInfo_T writableInfos[R_PVPCS_SOFAR_50KW_WRITE_DATA_INFOS_NUM] = {
        [W_PVPCS_SOFAR_50KW_ACTIVE_POWER_PRECENTAGE_SET] =
                {
                        .registerInfo =
                                {
                                        .startAddr = 0x0901,
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
                                        .write_data_get = pvpcs_sofar_50kw_active_power_precentage_set_write_data_get,
                                },
                },
};

ModbusMasterDataProtocol_T pvpcsSofar50kwDataProtocol = {
        .protocolsName        = "pvpcs sofar 50kw",
        .pReadableSyncInfos   = readableInfos,
        .readableSyncInfosNum = R_PVPCS_SOFAR_50KW_READ_DATA_INFOS_NUM,
        .pWritableSyncInfos   = writableInfos,
        .writableSyncInfosNum = R_PVPCS_SOFAR_50KW_WRITE_DATA_INFOS_NUM,
        .time =
                {
                        .frameInterval    = 10,
                        .retryInterval    = 10 * 1000,
                        .delayCardinality = 1,
                        .timeoutThreshold = 5 * 1000,
                },
        .linkApi =
                {
                        .lose    = pvpcs_sofar_50kw_link_lose,
                        .online  = NULL,
                        .connect = pvpcs_sofar_50kw_link_connect,
                },
};
