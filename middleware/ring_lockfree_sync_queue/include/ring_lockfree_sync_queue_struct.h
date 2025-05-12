//
// Created by three on 2025/5/4.
//

#ifndef RING_LOCKFREE_SYNC_QUEUE_STRUCT_H
#define RING_LOCKFREE_SYNC_QUEUE_STRUCT_H

#include <stdint.h>
#include <stdatomic.h>

typedef struct
{
        atomic_uchar write;
        atomic_uchar read;
}RingLockfreeUnitHolder_T;

typedef struct
{
        RingLockfreeUnitHolder_T holder;
        void *pBuffer;//动态申请的缓存
}RingLockfreeUnit_T;

typedef struct
{
        atomic_ushort initializeState;//0:未初始化；0xAA55:初始化完成；else:初始化中
        atomic_uint latestUnitIdx;//最新的数据单元的索引
        uint32_t unitSize;//单元内存大小
        uint32_t unitNum;//单元数量
        uint32_t timeoutWaitTime;//超时等待时间(ms)
        RingLockfreeUnit_T *pUnits;//单元组
}RingLockfreeSyncQueue_T;

#endif //RING_LOCKFREE_SYNC_QUEUE_STRUCT_H
