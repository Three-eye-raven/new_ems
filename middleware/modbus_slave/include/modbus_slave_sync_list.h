//
// Created by three on 2025/5/6.
//

#ifndef MODBUS_SLAVE_SYNC_LIST_H
#define MODBUS_SLAVE_SYNC_LIST_H

#include "ring_lockfree_sync_queue_struct.h"

#include <stdatomic.h>

#define MODBUS_SLAVE_SYNC_LIST_ASSERT

#define MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM 10
#define MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME 50

typedef struct
{
        uint16_t start;
        uint16_t length;
        uint16_t end;
}ModbusSlaveAddressInterval_T;

typedef struct
{
        atomic_bool initializedState;
        uint32_t cellSize;
        uint32_t intervalNum;//区间数量
        ModbusSlaveAddressInterval_T *pIntervals;
        RingLockfreeSyncQueue_T *pQueue;
}ModbusSlaveSyncList_T;


uint16_t modbus_slave_sync_list_init(ModbusSlaveSyncList_T* pList);
void modbus_slave_sync_list_deinit(const ModbusSlaveSyncList_T* pList);

uint16_t modbus_slave_sync_list_data_get(const ModbusSlaveSyncList_T* pList, const uint16_t address,
                                         const uint16_t length, void* pDst);

#endif //MODBUS_SLAVE_SYNC_LIST_H
