//
// Created by three on 2025/5/7.
//

#ifndef MODBUS_SLAVE_H
#define MODBUS_SLAVE_H

#include "modbus_slave_mapping.h"

typedef enum
{
        MODBUS_SLAVE_RTU_RS232 = 0,
        MODBUS_SLAVE_RTU_RS485 = 1,
}MODBUS_SLAVE_RTU_SERIAL_MODE_E;

typedef struct
{
        char     ipv4[16]; /*!< ipv4地址 */
        uint16_t port;     /*!< 端口 */
        int32_t maxConnectionNum; /*!< 最大链接数量 */
}ModbusSlaveTcpConfig_T;

typedef struct
{
        char                            serialName[32]; /*!< 设备路径名 */
        int                             baud;           /*!< 波特率 */
        char                            parity;         /*!< 奇偶校验位 */
        int                             dataBit;        /*!< 数据位长度 */
        int                             stopBit;        /*!< 停止位长度 */
        MODBUS_SLAVE_RTU_SERIAL_MODE_E mode;           /*!< 串口模式(485或232) */
}ModbusSlaveRtuConfig_T;

typedef struct
{
        ModbusSlaveTcpConfig_T config;
        ModbusSlaveChannelInfo_T channelInfo;
}ModbusSlaveTcpChannel_T;

typedef struct
{
        ModbusSlaveRtuConfig_T config;
        ModbusSlaveChannelInfo_T channelInfo;
}ModbusSlaveRtuChannel_T;

typedef struct
{
        int32_t client_socket;
        ModbusSlaveChannelInfo_T *pChannelInfo;
}ModbusSlaveTcpThreadTransParam_T;

void* modbus_slave_tcp_thread(void *arg);
void* modbus_slave_rtu_thread(void *arg);

#endif //MODBUS_SLAVE_H
