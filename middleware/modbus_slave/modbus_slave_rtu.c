//
// Created by three on 2025/5/7.
//
#include "modbus_slave.h"
#include "modbus_slave_mapping.h"
#include "modbus_slave_sync_list.h"

void* modbus_slave_rtu_thread(void *arg)
{
        const ModbusSlaveRtuChannel_T channel = *(ModbusSlaveRtuChannel_T*)arg;
        modbus_t *pCtx = modbus_new_rtu(channel.config.serialName,channel.config.baud,channel.config.parity,channel.config.dataBit,channel.config.stopBit);
        if (NULL == pCtx) {
                return NULL;
        }
        modbus_set_slave(pCtx,channel.channelInfo.slaveID);
        modbus_rtu_set_serial_mode(pCtx,channel.config.mode);
        if (-1 == modbus_connect(pCtx)) {
                modbus_free(pCtx);
                return NULL;
        }
        while (1) {
                const uint8_t rc = modbus_slave_rtu_reply(pCtx,&channel.channelInfo);
                if ((1 << 1) == rc) {
                        break; //接收时遇到超时以外的错误，断开链接
                }
                if ((1 << 4) == rc) {
                        break; //发送返回信息失败，断开链接
                }
        }
        modbus_close(pCtx);
        modbus_free(pCtx);
        return NULL;
}