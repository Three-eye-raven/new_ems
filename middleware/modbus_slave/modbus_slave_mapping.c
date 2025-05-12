//
// Created by three on 2025/4/27.
//
#include "modbus_slave_mapping.h"

#include "modbus.h"

#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

static modbus_mapping_t empty_mapping = {
        .nb_bits               = 0,
        .nb_input_bits         = 0,
        .nb_input_registers    = 0,
        .nb_registers          = 0,
        .start_bits            = 0,
        .start_input_bits      = 0,
        .start_input_registers = 0,
        .start_registers       = 0,
        .tab_bits              = NULL,
        .tab_input_bits        = NULL,
        .tab_input_registers   = NULL,
        .tab_registers         = NULL,
};

uint8_t modbus_slave_tcp_reply(modbus_t* pCtx, const ModbusSlaveChannelInfo_T* pChannelInfo)
{
        uint8_t ret = 0;
        if (NULL == pCtx || NULL == pChannelInfo || NULL == pChannelInfo->pBuffer) {
                ret |= (1 << 0);   // 输入参数不合法
                goto exit;
        }
        uint8_t       req[MODBUS_TCP_MAX_ADU_LENGTH];
        const int32_t rc = modbus_receive(pCtx, req);
        if (rc == -1) {
                if (errno != ETIMEDOUT) {
                        ret |= (1 << 1);   // 遇到超时以外的错误，连接断开
                        goto exit;
                }
                else {
                        ret |= (1 << 2);   // 链接超时
                        goto exit;
                }
        }
        // 提取从机id 功能码 数据地址 数据长度
        const uint8_t  funCode = req[7];
        const uint16_t dataAddress =
                (req[8]
                << 8) + req[9];
        uint16_t dataLength =
                (req[10]
                << 8) + req[11];
#if 0
        const uint8_t  slaveId = req[MODBUS_SLAVE_RC_SLAVE_ID_IDX + MODBUS_SLAVE_TCP_RC_OFFSET];
        if (slaveId != pChannelInfo->slaveID) {
                // 设备不存在，不响应
                ret |= (1 << 3);
                goto exit;
        }
#endif
        uint16_t         u16temp[MODBUS_MAX_READ_REGISTERS];
        modbus_mapping_t mapping = {
                .start_bits            = dataAddress,
                .start_input_bits      = dataAddress,
                .start_input_registers = dataAddress,
                .start_registers       = dataAddress,
                .nb_bits               = MODBUS_MAX_READ_BITS / 8,
                .nb_input_bits         = MODBUS_MAX_READ_BITS / 8,
                .nb_input_registers    = MODBUS_MAX_READ_REGISTERS,
                .nb_registers          = MODBUS_MAX_READ_REGISTERS,
                .tab_bits              = (uint8_t*)u16temp,
                .tab_input_bits        = (uint8_t*)u16temp,
                .tab_input_registers   = u16temp,
                .tab_registers         = u16temp,
        };
        modbus_mapping_t*      pMapping  = NULL;
        ModbusSlaveSyncList_T* pList     = NULL;
        int8_t                 writeType = -1;
        uint8_t*               pSrc      = NULL;
        // 根据不同的功能码匹配不同的数据同步表
        switch (funCode) {
        case 0x01 ... 0x04:
                // 读功能
                pList = pChannelInfo->pBuffer[funCode - 1];
                break;
        case 0x05:
                // 写单个线圈
                pList      = pChannelInfo->pBuffer[MODBUS_SLAVE_COIL_STATUS];
                dataLength = 1;
                writeType  = 0;
                pSrc       = &req[MODBUS_SLAVE_RC_DATA_LENGTH_H_INDEX + MODBUS_SLAVE_TCP_RC_OFFSET];
                break;
        case 0x06:
                // 写单个寄存器
                pList      = pChannelInfo->pBuffer[MODBUS_SLAVE_HOLDING_REGISTER];
                dataLength = 1;
                writeType  = 1;
                pSrc       = &req[MODBUS_SLAVE_RC_DATA_LENGTH_H_INDEX + MODBUS_SLAVE_TCP_RC_OFFSET];
                break;
        case 0x0F:
                // 写多个线圈
                pList     = pChannelInfo->pBuffer[MODBUS_SLAVE_COIL_STATUS];
                writeType = 0;
                pSrc      = &req[MODBUS_SLAVE_RC_DATA_SRC_START_INDEX + MODBUS_SLAVE_TCP_RC_OFFSET];
                break;
        case 0x10:
                // 写多个寄存器
                pList     = pChannelInfo->pBuffer[MODBUS_SLAVE_HOLDING_REGISTER];
                writeType = 1;
                pSrc      = &req[MODBUS_SLAVE_RC_DATA_SRC_START_INDEX + MODBUS_SLAVE_TCP_RC_OFFSET];
                break;
        default: break;
        }
        if (NULL == pList) {
                pMapping = &empty_mapping;
        }
        else {
                const uint16_t reply =
                        modbus_slave_sync_list_data_get(pList, dataAddress, dataLength, u16temp);
                if (0 == reply) {
                        pMapping = &mapping;
                }
                else {
                        pMapping = &empty_mapping;
                }
        }
        const int32_t rc2 = modbus_reply(pCtx, req, rc, pMapping);
        if (-1 == rc2) {
                ret |= (1 << 4);   // 发送失败
        }
        if (-1 != writeType) {
                if (NULL != pChannelInfo->writeApi[writeType].cmd_analyse) {
                        pChannelInfo->writeApi[writeType].cmd_analyse(
                                dataAddress, dataLength, pSrc, pChannelInfo->channelId);
                }
        }
exit:
        return ret;
}

uint8_t modbus_slave_rtu_reply(modbus_t* pCtx, const ModbusSlaveChannelInfo_T* pChannelInfo)
{
        uint8_t ret = 0;
        if (NULL == pCtx || NULL == pChannelInfo || NULL == pChannelInfo->pBuffer) {
                ret |= (1 << 0);   // 输入参数不合法
                goto exit;
        }
        uint8_t       req[MODBUS_RTU_MAX_ADU_LENGTH];
        const int32_t rc = modbus_receive(pCtx, req);
        if (rc == -1) {
                if (errno != ETIMEDOUT) {
                        ret |= (1 << 1);   // 遇到超时以外的错误，连接断开
                        goto exit;
                }
        }
        // 提取从机id 功能码 数据地址 数据长度
        const uint8_t  slaveId     = req[MODBUS_SLAVE_RC_SLAVE_ID_IDX];
        const uint8_t  funCode     = req[MODBUS_SLAVE_RC_FUN_CODE_INDEX];
        const uint16_t dataAddress = req[MODBUS_SLAVE_RC_ADDRESS_H_INDEX]
                                     << 8 + req[MODBUS_SLAVE_RC_ADDRESS_L_INDEX];
        uint16_t dataLength = req[MODBUS_SLAVE_RC_DATA_LENGTH_H_INDEX]
                              << 8 + req[MODBUS_SLAVE_RC_DATA_LENGTH_L_INDEX];
#if 1
        if (slaveId != pChannelInfo->slaveID) {
                // slave id 不匹配，不响应
                ret |= (1 << 2);
                goto exit;
        }
#endif
        uint16_t         u16temp[MODBUS_MAX_READ_REGISTERS];
        modbus_mapping_t mapping = {
                .start_bits            = dataAddress,
                .start_input_bits      = dataAddress,
                .start_input_registers = dataAddress,
                .start_registers       = dataAddress,
                .nb_bits               = MODBUS_MAX_READ_BITS / 8,
                .nb_input_bits         = MODBUS_MAX_READ_BITS / 8,
                .nb_input_registers    = MODBUS_MAX_READ_REGISTERS,
                .nb_registers          = MODBUS_MAX_READ_REGISTERS,
                .tab_bits              = (uint8_t*)u16temp,
                .tab_input_bits        = (uint8_t*)u16temp,
                .tab_input_registers   = u16temp,
                .tab_registers         = u16temp,
        };
        modbus_mapping_t*      pMapping  = NULL;
        ModbusSlaveSyncList_T* pList     = NULL;
        int8_t                 writeType = -1;
        uint8_t*               pSrc      = NULL;
        // 根据不同的功能码匹配不同的数据同步表
        switch (funCode) {
        case 0x01 ... 0x04:
                // 读功能
                pList = pChannelInfo->pBuffer[funCode - 1];
                break;
        case 0x05:
                // 写单个线圈
                pList      = pChannelInfo->pBuffer[MODBUS_SLAVE_COIL_STATUS];
                dataLength = 1;
                writeType  = 0;
                pSrc       = &req[MODBUS_SLAVE_RC_DATA_LENGTH_H_INDEX];
                break;
        case 0x06:
                // 写单个寄存器
                pList      = pChannelInfo->pBuffer[MODBUS_SLAVE_HOLDING_REGISTER];
                dataLength = 1;
                writeType  = 1;
                pSrc       = &req[MODBUS_SLAVE_RC_DATA_LENGTH_H_INDEX];
                break;
        case 0x0F:
                // 写多个线圈
                pList     = pChannelInfo->pBuffer[MODBUS_SLAVE_COIL_STATUS];
                writeType = 0;
                pSrc      = &req[MODBUS_SLAVE_RC_DATA_SRC_START_INDEX];
                break;
        case 0x10:
                // 写多个寄存器
                pList      = pChannelInfo->pBuffer[MODBUS_SLAVE_HOLDING_REGISTER];
                dataLength = 1;
                pSrc       = &req[MODBUS_SLAVE_RC_DATA_SRC_START_INDEX];
                break;
        default: break;
        }
        if (NULL == pList) {
                pMapping = &empty_mapping;
        }
        else {
                const uint16_t reply =
                        modbus_slave_sync_list_data_get(pList, dataAddress, dataLength, u16temp);
                if (0 == reply) {
                        pMapping = &mapping;
                }
                else {
                        pMapping = &empty_mapping;
                }
        }
        const int32_t rc2 = modbus_reply(pCtx, req, rc, pMapping);
        if (-1 == rc2) {
                ret |= (1 << 3);   // 发送失败
        }
        if (-1 != writeType) {
                if (NULL != pChannelInfo->writeApi[writeType].cmd_analyse) {
                        pChannelInfo->writeApi[writeType].cmd_analyse(
                                dataAddress, dataLength, pSrc, pChannelInfo->channelId);
                }
        }
exit:
        return ret;
}
