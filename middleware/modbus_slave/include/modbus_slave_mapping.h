//
// Created by three on 2025/4/27.
//

#ifndef MODBUS_SLAVE_MAPPING_H
#define MODBUS_SLAVE_MAPPING_H

#include "modbus_slave_sync_list.h"

#include "modbus.h"

#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

#define MODBUS_SLAVE_NUM_MAX 0xFF

#define MODBUS_SLAVE_TCP_RC_OFFSET 6

#define MODBUS_SLAVE_RC_SLAVE_ID_IDX 0
#define MODBUS_SLAVE_RC_FUN_CODE_INDEX 1
#define MODBUS_SLAVE_RC_ADDRESS_H_INDEX 2
#define MODBUS_SLAVE_RC_ADDRESS_L_INDEX 3
#define MODBUS_SLAVE_RC_DATA_LENGTH_H_INDEX 4
#define MODBUS_SLAVE_RC_DATA_LENGTH_L_INDEX 5
#define MODBUS_SLAVE_RC_DATA_SRC_START_INDEX 7

#define MODBUS_SLAVE_REGISTER_TPYE_NUM 4

typedef enum
{
        MODBUS_SLAVE_COIL_STATUS,      /*!< 线圈 */
        MODBUS_SLAVE_INPUT_STATUS,     /*!< 离散输入 */
        MODBUS_SLAVE_HOLDING_REGISTER, /*!< 保持寄存器 */
        MODBUS_SLAVE_INPUT_REGISTER,   /*!< 输入寄存器 */
} MODBUS_SLAVE_REGISTER_TYPE_E;

typedef struct
{
        void (*cmd_analyse)(uint16_t address,uint16_t length,uint8_t *pSrc,uint16_t channlId);
}ModbusSlaveRegisterWriteApi_T;

typedef struct
{
        uint8_t slaveID;
        uint16_t channelId;
        ModbusSlaveSyncList_T *pBuffer[MODBUS_SLAVE_REGISTER_TPYE_NUM];
        ModbusSlaveRegisterWriteApi_T writeApi[2];
} ModbusSlaveChannelInfo_T;

uint8_t modbus_slave_tcp_reply(modbus_t* pCtx, const ModbusSlaveChannelInfo_T* pChannelInfo);
uint8_t modbus_slave_rtu_reply(modbus_t* pCtx, const ModbusSlaveChannelInfo_T* pChannelInfo);

#endif   // MODBUS_SLAVE_MAPPING_H
