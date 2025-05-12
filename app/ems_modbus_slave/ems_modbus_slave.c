//
// Created by three on 2025/5/7.
//
#include "ems_modbus_slave.h"

#include "modbus_slave.h"

extern ModbusSlaveSyncList_T emsHolderRegisterSyncList;

static ModbusSlaveTcpChannel_T emsChannel = {
        .config = {
                .ipv4 = "127.0.0.1",
                .port = 1502,
                .maxConnectionNum = 5,
        },
        .channelInfo = {
                .slaveID = 0x01,
                .channelId = 0,
                .pBuffer = {
                        [MODBUS_SLAVE_COIL_STATUS] = NULL,
                        [MODBUS_SLAVE_INPUT_STATUS] = NULL,
                        [MODBUS_SLAVE_HOLDING_REGISTER] = &emsHolderRegisterSyncList,
                        [MODBUS_SLAVE_INPUT_REGISTER] = NULL,
                },
                .writeApi = {
                        [0] = NULL,
                        [1] = NULL,
                },
        }
};

static ModbusSlaveTcpChannel_T webChannel = {
        .config = {
                .ipv4 = "127.0.0.1",
                .port = 9998,
                .maxConnectionNum = 5,
        },
        .channelInfo = {
                .slaveID = 0x01,
                .channelId = 1,
                .pBuffer = {
                        [MODBUS_SLAVE_COIL_STATUS] = NULL,
                        [MODBUS_SLAVE_INPUT_STATUS] = NULL,
                        [MODBUS_SLAVE_HOLDING_REGISTER] = &emsHolderRegisterSyncList,
                        [MODBUS_SLAVE_INPUT_REGISTER] = NULL,
                },
                .writeApi = {
                        [0] = NULL,
                        [1] = NULL,
                },
        }
};

static ModbusSlaveTcpChannel_T pcChannel = {
        .config = {
                .ipv4 = "127.0.0.1",
                .port = 9997,
                .maxConnectionNum = 5,
        },
        .channelInfo = {
                .slaveID = 0x01,
                .channelId = 2,
                .pBuffer = {
                        [MODBUS_SLAVE_COIL_STATUS] = NULL,
                        [MODBUS_SLAVE_INPUT_STATUS] = NULL,
                        [MODBUS_SLAVE_HOLDING_REGISTER] = &emsHolderRegisterSyncList,
                        [MODBUS_SLAVE_INPUT_REGISTER] = NULL,
                },
                .writeApi = {
                        [0] = NULL,
                        [1] = NULL,
                },
        }
};

static pthread_t emsChannelThread;
static pthread_t webChannelThread;
static pthread_t pcChannelThread;

bool ems_modbus_slave_thread_init(void)
{
        bool     ret = false;
        const uint16_t u16Reply = emsHolderRegisterSyncListInit();
        if (0 == u16Reply) {
                int32_t i32Reply = 0;
                i32Reply = pthread_create(&emsChannelThread,NULL,modbus_slave_tcp_thread,&emsChannel);
                if (0 != i32Reply) {
                        goto exit;
                }
#if 1
                i32Reply = pthread_create(&webChannelThread,NULL,modbus_slave_tcp_thread,&webChannel);
                if (0 != i32Reply) {
                        goto exit;
                }
                i32Reply = pthread_create(&pcChannelThread,NULL,modbus_slave_tcp_thread,&pcChannel);
                if (0 != i32Reply) {
                        goto exit;
                }
#endif
                ret = true;
        }
        exit:
        if (false == ret) {
                emsHolderRegisterSyncListDeinit();
        }
        return ret;
}

void ems_modbus_slave_thread_join(void)
{
        pthread_join(emsChannelThread,NULL);
#if 1
        pthread_join(webChannelThread,NULL);
        pthread_join(pcChannelThread,NULL);
#endif
}

