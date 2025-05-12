/**
* @file
 * @brief       modbus master 函数接口声明
 * @details     包含rtu、tcp信道的初始化、重连、反初始化，读写数据的同步
 * @author      ylb
 * @date        2025-04-25
 * @version     v1.0
 * @par History:
 *	version v1.0: 基础程序首次创建\n
 */

#ifndef MODBUS_MASTER_API_H
#define MODBUS_MASTER_API_H

#include "modbus_master_struct.h"

bool modbus_master_tcp_channel_init(ModbusMasterTcpChannel_T* pChannel);
void modbus_master_tcp_channel_deinit(ModbusMasterTcpChannel_T* pChannel);

bool modbus_master_rtu_channel_init(ModbusMasterRtuChannel_T* pChannel);
void modbus_master_rtu_channel_deinit(ModbusMasterRtuChannel_T* pChannel);

bool modbus_master_channel_reconnect(ModbusMasterChannelInfo_T* pChannel_info);

uint8_t modbus_master_slave_readable_infos_sync(ModbusMasterChannelInfo_T* pChannel,
                                                const uint8_t              slaveIdx);
uint8_t modbus_master_slave_writable_infos_sync(ModbusMasterChannelInfo_T* pChannel,
                                                const uint8_t              slaveIdx);

#endif   // MODBUS_MASTER_API_H
