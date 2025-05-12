//
// Created by 谷子 on 2025/4/8.
//
#include "pvpcs_cfg.h"

#include "modbus_master_api.h"
#include "modbus.h"

#include "pvpcs_data_interaction_api.h"
#include "pvpcs_hw_110kw/pvpcs_hw_110kw_data_protocol.h"

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

static ModbusMasterRtuChannel_T rtuChannel = {
        .config = {
                .serialName = "/dev/ttyRS485-3",
                .baud = 9600,
                .parity = 'N',
                .dataBit = 8,
                .stopBit = 1,
                .mode = MODBUS_MASTER_RTU_RS485,
        },
        .channelInfo = {
                .channelName = "RS485-3",
                .pCtx = NULL,
                .initialized = false,
                .slaveNum = 1,
                .slaveInfo = {
                        [0] = {
                                .deviceId = 0,
                                .slaveId = 0x01,
                                .pProtocol = &pvpcsHw110kwDataProtocol,
                                .linkState = {0},
                        },
                },
        },
};

void *pvpcs_rtu_communication_thread(void *arg)
{
    bool ret = modbus_master_rtu_channel_init(&rtuChannel);
    ModbusMasterChannelInfo_T *channle_info = &rtuChannel.channelInfo;
    if (!ret)
    {
        printf("modbus_master_rtu_channel_init failed\n");
    }
    else
    {
        uint16_t active_power           = 0;
        uint32_t run_time               = 0;
        uint32_t total_power_generation = 0;
        uint32_t daily_power_generation = 0;
        uint32_t max_active_power = 0;
        printf("modbus_master_rtu_channel_init success\n");
        //modbus_set_debug(channle_info->pCtx,true);
        modbus_set_response_timeout(channle_info->pCtx,1,0);
        do
        {
            daily_power_generation = 0;
            total_power_generation = 0;
            active_power = 0;
            max_active_power = 0;
            for(uint8_t device_id = 0; device_id < channle_info->slaveNum; device_id++)
            {
                //数据获取
                modbus_master_slave_readable_infos_sync(channle_info, device_id);
                //数据整理
                daily_power_generation += pvpcs_daily_power_generation_get(device_id);
                total_power_generation += pvpcs_total_power_generation_get(device_id);
                active_power += pvpcs_total_active_power_get(device_id);
                max_active_power += pvpcs_max_active_power_get(device_id);
                run_time = channle_info->slaveInfo[device_id].linkState.lastHandshakeTime - channle_info->slaveInfo[device_id].linkState.firstHandshakeTime;
                run_time /= (1000 * 3600);
                pvpcs_run_time_set(device_id, run_time);
                //todo:判断设备是否故障，若故障，尝试复位
                //指令下发
                //debug输出获取的数据
                print_pvpcs_single_data(device_id);
            }
            pvpcs_multi_daily_power_generation_set(daily_power_generation);//写入设备组总每日发电量
            pvpcs_multi_total_power_generation_set(total_power_generation);//写入设备组总发电量
            pvpcs_multi_active_power_set(active_power);//写入设备组当前总有功功率
            pvpcs_multi_max_active_power_set(max_active_power);//写入设备组最大有功功率
            //若所有设备掉线，重新初始化通道
            if (pvpcs_connect_status_get() == 0) {
                //重新链接通道
            }
            usleep(100*1000);
        }while(1);
    }
    return NULL;
}

static pthread_t pvpcs_rtu_communication_thread_id;


bool pvpcs_rtu_communication_thread_init(void)
{
    bool ret = true;
    if(pthread_create(&pvpcs_rtu_communication_thread_id, NULL, pvpcs_rtu_communication_thread, NULL) !=0)
    {
        ret = false;
    }
    return ret;
}

void pvpcs_rtu_communication_thread_join(void)
{
    pthread_join(pvpcs_rtu_communication_thread_id, NULL);
}
