//
// Created by three on 2025/5/6.
//
#include "modbus_slave_sync_list.h"

#include "ring_lockfree_sync_queue_api.h"

#include <stdatomic.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#ifdef MODBUS_SLAVE_SYNC_LIST_ASSERT
#        include "assert.h"
#endif

#if 0
ModbusSlaveSyncList_T* modbus_slave_sync_list_new(const uint32_t intervalNum,
                                                  const uint32_t cellSize)
{
        ModbusSlaveSyncList_T* pList = NULL;
#ifdef MODBUS_SLAVE_SYNC_LIST_ASSERT
        assert(0 != intervalNum);
        assert(0 != cellSize);
#else
        if (0 == intervalNum) {
                goto exit;
        }
        if (0 == cellSize) {
                goto exit;
        }
#endif
        pList = (ModbusSlaveSyncList_T*)malloc(sizeof(ModbusSlaveSyncList_T));
        if (NULL == pList) {
                goto exit;
        }
        pList->pIntervals = (ModbusSlaveAddressInterval_T*)malloc(
                intervalNum * sizeof(ModbusSlaveAddressInterval_T));
        if (NULL == pList->pIntervals) {
                goto exit;
        }
        pList->pQueue =
                (RingLockfreeSyncQueue_T*)malloc(intervalNum * sizeof(RingLockfreeSyncQueue_T));
        if (NULL == pList->pQueue) {
                goto exit;
        }
        pList->intervalNum = intervalNum;
        pList->cellSize    = cellSize;
        atomic_store(&pList->initializedState, false);
exit:
        if (NULL != pList) {
                if (NULL == pList->pQueue || NULL == pList->pIntervals) {
                        if (NULL != pList->pQueue) {
                                free(pList->pQueue);
                                pList->pQueue = NULL;
                        }
                        if (NULL != pList->pIntervals) {
                                free(pList->pIntervals);
                                pList->pIntervals = NULL;
                        }
                        free(pList);
                        pList = NULL;
                }
        }
        return pList;
}

uint16_t modbus_slave_interval_config(ModbusSlaveSyncList_T* pList, const uint32_t intervalIdx,
                                      const uint16_t startAddress, const uint16_t length)
{
        uint16_t ret = 0;
#ifdef MODBUS_SLAVE_SYNC_LIST_ASSERT
        assert(NULL != pList);
        assert(NULL != pList->pIntervals);
        assert(intervalIdx < pList->intervalNum);
        assert(0 != length);
#else
        if (NULL == pList) {
                ret |= (1 << 0);
                goto exit;
        }
        if (NULL == pList->pIntervals) {
                ret |= (1 << 0);
                goto exit;
        }
        if (intervalIdx >= pList->intervalNum) {
                ret |= (1 << 0);
                goto exit;
        }
        if (0 == length) {
                ret |= (1 << 0);
                goto exit;
        }
#endif
        const uint32_t end = startAddress + length - 1;
        if (end > 0xFFFF) {
                ret |= (1 << 2);   // 长度过长
                goto exit;
        }
        ModbusSlaveAddressInterval_T* pInterval = &pList->pIntervals[intervalIdx];
        pInterval->start                        = startAddress;
        pInterval->length                       = length;
        pInterval->end                          = end;
exit:
        return ret;
}

uint16_t modbus_slave_sync_queue_config(ModbusSlaveSyncList_T* pList, const uint32_t intervalIdx,
                                        const uint32_t unitNum, const uint32_t timeoutWaitTime)
{
        uint16_t ret = 0;
#ifdef MODBUS_SLAVE_SYNC_LIST_ASSERT
        assert(NULL != pList);
        assert(NULL != pList->pIntervals);
        assert(NULL != pList->pQueue);
        assert(0 != pList->cellSize);
        assert(0 != pList->intervalNum);
        assert(intervalIdx < pList->intervalNum);
        assert(pList->pIntervals[intervalIdx].start <= pList->pIntervals[intervalIdx].end);
#else
        if (NULL == pList) {
                ret |= (1 << 0);
                goto exit;
        }
        if (NULL == pList->pQueue || NULL == pList->pIntervals) {
                ret |= (1 << 0);
                goto exit;
        }
        if (0 == pList->cellSize || 0 == pList->intervalNum) {
                ret |= (1 << 0);
                goto exit;
        }
        if (intervalIdx >= pList->intervalNum) {
                ret |= (1 << 0);
                goto exit;
        }
        if (0 == pList->pIntervals[intervalIdx].length) {
                ret |= (1 << 0);
                goto exit;
        }
#endif
        const ModbusSlaveAddressInterval_T* pInterval = &pList->pIntervals[intervalIdx];
        RingLockfreeSyncQueue_T*            pQueue    = &pList->pQueue[intervalIdx];
        pQueue->unitSize                              = pInterval->length * pList->cellSize;
        pQueue->unitNum                               = unitNum;
        pQueue->timeoutWaitTime                       = timeoutWaitTime;
exit:
        return ret;
}

void modbus_slave_sync_list_free(ModbusSlaveSyncList_T* pList)
{
#ifdef MODBUS_SLAVE_SYNC_LIST_ASSERT
        assert(NULL != pList);
#else
        if (NULL == pList) {
                return;
        }
#endif
        if (NULL != pList->pIntervals) {
                free(pList->pIntervals);
                pList->pIntervals = NULL;
        }
        if (NULL != pList->pQueue) {
                for (uint32_t idx = 0; idx < pList->intervalNum; ++idx) {
                        ring_lockfree_sync_queue_deinit(&pList->pQueue[idx]);
                }
                free(pList->pQueue);
                pList->pQueue = NULL;
        }
        free(pList);
        pList = NULL;
}

#endif

/**
 * @brief modbus slave 同步表区间配置检查
 * @param pList modbus slave 同步表
 *
 * @return 0:正常读同步
 *         bit0:输入参数不合法
 *         bit1:区间参数不合法
 */
static uint16_t modbus_slave_sync_list_interval_config_check(ModbusSlaveSyncList_T* pList)
{
        uint16_t ret = 0;
#ifdef MODBUS_SLAVE_SYNC_LIST_ASSERT
        assert(NULL != pList);
        assert(NULL != pList->pIntervals);
        assert(0 != pList->intervalNum);
#else
        if (NULL == pList) {
                ret |= (1 << 0);
                goto exit;
        }
        if (NULL == pList->pIntervals) {
                ret |= (1 << 0);
                goto exit;
        }
        if (0 == pList->intervalNum) {
                ret |= (1 << 0);
                goto exit;
        }
#endif
        uint32_t lastEnd = 0;
        for (uint32_t idx = 0; idx < pList->intervalNum; ++idx) {
                ModbusSlaveAddressInterval_T* pInterval = &pList->pIntervals[idx];
                const uint32_t end = pInterval->start + pInterval->length - 1;
                if (end > 0xFFFF) {
                        ret |= (1 << 1);
                        goto exit;
                }
                pInterval->end = end;
                if (pInterval->start > pInterval->end) {
                        ret |= (1 << 1);   // 区间不合法
                        goto exit;
                }
                if (0 != idx) {
                        if (lastEnd >= pInterval->start) {
                                ret |= (1 << 1);   // 区间不合法
                                goto exit;
                        }
                }
                lastEnd = end;
        }
exit:
        return ret;
}

/**
 * @brief modbus slave 同步表初始化
 * @param pList modbus slave 同步表
 *
 * @return 0:正常读同步
 *         bit0:输入参数不合法
 *         bit1:区间参数不合法
 */
uint16_t modbus_slave_sync_list_init(ModbusSlaveSyncList_T* pList)
{
        uint16_t ret = 0;
#ifdef MODBUS_SLAVE_SYNC_LIST_ASSERT
        assert(NULL != pList);
        assert(NULL != pList->pIntervals);
        assert(NULL != pList->pQueue);
        assert(0 != pList->intervalNum);
#else
        if (NULL == pList) {
                ret |= (1 << 0);
                goto exit;
        }
        if (NULL == pList->pQueue || NULL == pList->pIntervals) {
                ret |= (1 << 0);
                goto exit;
        }
        if (0 == pList->intervalNum) {
                ret |= (1 << 0);
                goto exit;
        }
#endif
        //区间合法性检查，要求递增且不重叠
        uint16_t reply = modbus_slave_sync_list_interval_config_check(pList);
        if (0 != reply) {
                ret |= (1 << 1);
        }
        for (uint32_t idx = 0; idx < pList->intervalNum; ++idx) {
                pList->pQueue[idx].unitSize = pList->pIntervals[idx].length * pList->cellSize;
                reply = ring_lockfree_sync_queue_init(&pList->pQueue[idx]);
                if (0 != reply) {
                        ret |= (1 << 2);
                        for (uint32_t idx2 = 0; idx2 < idx; idx2++) {
                                ring_lockfree_sync_queue_deinit(&pList->pQueue[idx2]);
                        }
                        goto exit;
                }
                atomic_store(&pList->initializedState, true);
        }
exit:
        return ret;
}

void modbus_slave_sync_list_deinit(const ModbusSlaveSyncList_T* pList)
{
#ifdef MODBUS_SLAVE_SYNC_LIST_ASSERT
        assert(NULL != pList);
        assert(NULL != pList->pIntervals);
        assert(NULL != pList->pQueue);
#else
        if (NULL == pList) {
                return;
        }
        if (NULL == pList->pIntervals) {
                return;
        }
        if (NULL == pList->pQueue) {
                return;
        }
#endif
        for (uint32_t idx = 0; idx < pList->intervalNum; ++idx) {
                ring_lockfree_sync_queue_deinit(&pList->pQueue[idx]);
        }
}

static int32_t modbus_slave_intervalIdx_find(const ModbusSlaveSyncList_T* pList,
                                             const uint16_t               address)
{
        int32_t ret = 0;
#ifdef MODBUS_SLAVE_SYNC_LIST_ASSERT
        assert(NULL != pList);
        assert(false != pList->initializedState);
        assert(0 != pList->intervalNum);
#else
        if (NULL == pList) {
                ret = -1;
                goto exit;
        }
        if (false == pList->initializedState) {
                ret = -1;
                goto exit;
        }
        if (0 == pList->intervalNum) {
                ret = -1;
                goto exit;
        }
#endif
        int32_t low = 0, high = pList->intervalNum;
        while (low < high) {
                const int32_t mid = (low + high) / 2;
                if (pList->pIntervals[mid].start > address) {
                        high = mid;
                }
                else {
                        low = mid + 1;
                }
        }
        const int32_t idx = low - 1;
        if (idx >= 0 && pList->pIntervals[idx].end >= address) {
                ret = idx;
                goto exit;
        }
        else {
                ret = -1;
                goto exit;
        }
exit:
        return ret;
}

static bool modbus_slave_intervalIdx_continuity_check(const ModbusSlaveSyncList_T* pList,
                                                      const int32_t startIdx, const int32_t endIdx)
{
        bool ret = false;
#ifdef MODBUS_SLAVE_SYNC_LIST_ASSERT
        assert(NULL != pList);
        assert(NULL != pList->pIntervals);
        assert(NULL != pList->pQueue);
        assert(0 != pList->cellSize && 0 != pList->intervalNum);
#else
        if (NULL == pList) {
                goto exit;
        }
        if (NULL == pList->pIntervals || NULL == pList->pQueue) {
                goto exit;
        }
        if (0 == pList->cellSize || 0 == pList->intervalNum) {
                goto exit;
        }
#endif
        if (-1 == startIdx || -1 == endIdx) {
                goto exit;
        }
        else if (startIdx == endIdx) {
                ret = true;   // 起点和结尾在同一区间中，必然是连续的
        }
        else {
                for (int32_t idx = startIdx; idx < endIdx; ++idx) {
                        if ((pList->pIntervals[idx].end + 1) != pList->pIntervals[idx + 1].start) {
                                goto exit;
                        }
                }
                ret = true;   // 起点到结尾间的区间首尾相连
        }
exit:
        return ret;
}

uint16_t modbus_slave_sync_list_data_get(const ModbusSlaveSyncList_T* pList, const uint16_t address,
                                         const uint16_t length, void* pDst)
{
        uint16_t ret = 0;
#ifdef MODBUS_SLAVE_SYNC_LIST_ASSERT
        assert(NULL != pList);
        assert(false != pList->initializedState);
        assert(NULL != pDst);
        assert(0 != length);
#else
        if (NULL == pList) {
                ret |= (1 << 0);
                goto exit;
        }
        if (false == pList->initializedState) {
                ret |= (1 << 0);
                goto exit;
        }
        if (NULL == pDst) {
                ret |= (1 << 0);
                goto exit;
        }
        if (0 == length) {
                ret |= (1 << 0);
                goto exit;
        }
#endif
        // 先执行连续性检查
        const int32_t startIdx = modbus_slave_intervalIdx_find(pList, address);
        const int32_t endIdx   = modbus_slave_intervalIdx_find(pList, address + length - 1);
        const bool checkReply  = modbus_slave_intervalIdx_continuity_check(pList, startIdx, endIdx);
        if (false == checkReply) {
                ret |= (1 << 1);   // 区间连续性检查未通过
                goto exit;
        }
        uint16_t reply      = 0;
        uint32_t size       = 0;
        uint32_t offset     = 0;
        uint16_t copyLength = length;
        if (startIdx == endIdx) {
                offset = (address - pList->pIntervals[startIdx].start) * pList->cellSize;
                size   = length * pList->cellSize;
                reply  = ring_lockfree_sync_queue_get(pList->pQueue + startIdx, pDst, offset, size);
                if (0 != reply) {
                        ret |= (1 << 2);   // 数据获取失败
                        goto exit;
                }
        }
        else {
                for (uint32_t idx = startIdx; idx <= endIdx; ++idx) {
                        if (startIdx == idx) {
                                offset = address - pList->pIntervals[idx].start;
                                size   = pList->pIntervals[idx].length - offset;
                        }
                        else if (endIdx == idx) {
                                offset = 0;
                                size   = copyLength;
                        }
                        else {
                                offset = 0;
                                size   = pList->pIntervals[idx].length;
                        }
                        const uint32_t copySize = size * pList->cellSize;
                        reply                   = ring_lockfree_sync_queue_get(
                                pList->pQueue + startIdx, pDst, offset * pList->cellSize, copySize);
                        if (0 != reply) {
                                ret |= (1 << 2);   // 数据获取失败
                                goto exit;
                        }
                        pDst += copySize;
                        copyLength -= size;
                }
        }
exit:
        return ret;
}
