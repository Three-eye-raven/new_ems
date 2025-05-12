//
// Created by three on 2025/5/6.
//
#include "ring_lockfree_sync_queue_api.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// #define RING_LOCKFREE_SYNC_QUEUE_ASSERT

#ifdef RING_LOCKFREE_SYNC_QUEUE_ASSERT
#        include <assert.h>
#endif

/**
 * @brief 初始化环形无锁同步队列
 * @param pQueue 环形无锁同步队列
 *
 * @return 0:正常初始化完成
 *         bit0:输入参数不合法
 *         bit1:已有其他线程占用初始化
 *         bit2:内存申请失败
 */
uint16_t ring_lockfree_sync_queue_init(RingLockfreeSyncQueue_T* pQueue)
{

        uint16_t ret = 0;
        // 输入参数检查
#ifdef RING_LOCKFREE_SYNC_QUEUE_ASSERT
        assert(NULL != pQueue);
        assert(0 != pQueue->unitSize);
        assert(0 != pQueue->unitNum);
#else
        if (NULL == pQueue) {
                ret |= (1 << 0);   // 输入参数不合法
                goto exit;
        }
        if (0 == pQueue->unitSize) {
                ret |= (1 << 0);
                goto exit;
        }
        if (0 == pQueue->unitNum) {
                ret |= (1 << 0);
                goto exit;
        }
#endif
        // 申请初始化权限
        const uint16_t initializeState = atomic_fetch_add(&pQueue->initializeState, 1);
        if (0 != initializeState) {
                ret |= (1 << 1);   // 已有他处开始初始化,退出本次初始化
                goto exit;
        }
        // 初始化权限申请成功，开始执行初始化
        // 申请数据块
        RingLockfreeUnit_T* pUnits = (RingLockfreeUnit_T*)malloc(pQueue->unitNum * sizeof(RingLockfreeUnit_T));
        if (NULL == pUnits) {
                ret |= (1 << 2);   // 内存申请失败
                goto exit;
        }
        for (uint32_t idx = 0; idx < pQueue->unitNum; ++idx) {
                // 申请数据块缓存
                pUnits[idx].pBuffer = (void*)malloc(pQueue->unitSize);
                if (NULL == pUnits[idx].pBuffer) {
                        ret |= (1 << 2);   // 内存申请失败
                        for (uint32_t i = 0; i < idx; i++) {
                                if (NULL != pUnits[i].pBuffer) {
                                        free(pUnits[i].pBuffer);
                                }
                        }
                        goto exit;
                }
                // 初始化读写占用符号
                atomic_store(&pUnits[idx].holder.read, 0);
                atomic_store(&pUnits[idx].holder.write, 0);
        }
        pQueue->pUnits = pUnits;
        atomic_store(&pQueue->initializeState, 0xAA55);
        atomic_store(&pQueue->latestUnitIdx, 0);
// 函数退出
exit:
        if (0 != ret) {
                if (NULL != pUnits) {
                        free(pUnits);
                }
                if (0 == (ret & (1 << 1))) {
                        atomic_store(&pQueue->initializeState, 0);
                }
        }
        return ret;
}

/**
 * @brief 反初始化环形无锁同步队列
 * @param pQueue 环形无锁同步队列
 *
 */
void ring_lockfree_sync_queue_deinit(RingLockfreeSyncQueue_T* pQueue)
{
#ifdef RING_LOCKFREE_SYNC_QUEUE_ASSERT
        assert(NULL != pQueue);
#else
        if (NULL == pQueue) {
                return;
        }
#endif
        if (NULL != pQueue->pUnits) {
                for (uint32_t idx = 0; idx < pQueue->unitNum; ++idx) {
                        if (NULL != pQueue->pUnits[idx].pBuffer) {
                                free(pQueue->pUnits[idx].pBuffer);
                        }
                }
                free(pQueue->pUnits);
        }
        atomic_store(&pQueue->initializeState, 0);
}

/**
 * @brief 申请并初始化环形无锁同步队列
 * @param unitSize 缓存单元大小
 * @param unitNum 缓存单元数量
 * @param timeoutWaitTime 超时等待时间
 *
 * @return NULL:申请失败
 *         else:申请成功
 */
RingLockfreeSyncQueue_T* ring_lockfree_sync_queue_new(const uint32_t unitSize,
                                                      const uint32_t unitNum,
                                                      const uint32_t timeoutWaitTime)
{
        RingLockfreeSyncQueue_T* ret = NULL;
#ifdef RING_LOCKFREE_SYNC_QUEUE_ASSERT
        assert(0 != unitSize);
        assert(0 != unitNum);
#else
        if (0 == unitNum || 0 == unitSize) {
                goto exit;
        }
#endif
        ret = (RingLockfreeSyncQueue_T*)malloc(sizeof(RingLockfreeSyncQueue_T));
        if (NULL == ret) {
                goto exit;
        }
        ret->unitSize        = unitSize;
        ret->unitNum         = unitNum;
        ret->timeoutWaitTime = timeoutWaitTime;
        atomic_store(&ret->initializeState, 0);
        const uint16_t initializeState = ring_lockfree_sync_queue_init(ret);
        if (0 != initializeState) {
                free(ret);
                ret = NULL;
                goto exit;
        }
exit:
        return ret;
}

/**
 * @brief 释放环形无锁同步队列
 * @param pQueue 环形无锁同步队列
 *
 */
void ring_lockfree_sync_queue_free(RingLockfreeSyncQueue_T* pQueue)
{
#ifdef RING_LOCKFREE_SYNC_QUEUE_ASSERT
        assert(NULL != pQueue);
#else
        if (NULL == pQueue) {
                return;
        }
#endif
        ring_lockfree_sync_queue_deinit(pQueue);
        free(pQueue);
}

/**
 * @brief 读环形无锁同步队列中的最新数据
 * @param pQueue 数据环形无锁同步队列
 * @param pDst 目标缓存区
 * @param offset 拷贝偏移量
 * @param size 拷贝大小
 *
 * @return 0:正常读同步
 *         bit0:输入参数不合法
 *         bit1:队列参数异常，不支持访问
 *         bit2:操作权限申请超时
 */
uint16_t ring_lockfree_sync_queue_get(RingLockfreeSyncQueue_T* pQueue, void* pDst,
                                      const uint32_t offset, const uint32_t size)
{
        uint16_t ret = 0;
#ifdef RING_LOCKFREE_SYNC_QUEUE_ASSERT
        assert(NULL != pQueue);
        assert(NULL != pDst);
        assert((offset + size) <= pQueue->unitSize);
#else
        if (NULL == pQueue) {
                ret |= (1 << 0);
                goto exit;
        }
        if (NULL == pDst) {
                ret |= (1 << 0);
                goto exit;
        }
        if ((offset + size) > pQueue->unitSize) {
                ret |= (1 << 0);
                goto exit;
        }
#endif
        const uint16_t initializeState = atomic_load(&pQueue->initializeState);
        if (0xAA55 != initializeState) {
                ret |= (1 << 1);   // 队列不支持访问
                goto exit;
        }
        uint32_t lastUnitIdx = atomic_load(&pQueue->latestUnitIdx);
        if (lastUnitIdx >= pQueue->unitNum) {
                ret |= (1 << 1);   // 队列不支持访问
                goto exit;
        }
        RingLockfreeUnit_T* pUnit = &pQueue->pUnits[lastUnitIdx];
        if (NULL == pUnit->pBuffer) {
                ret |= (1 << 1);   // 队列不支持访问
                goto exit;
        }
        // 申请读取占用符
        uint8_t  readHolder  = 0;
        uint8_t  writeHolder = 0;
        uint32_t waitingTime = 0;
        do {
                readHolder  = atomic_load(&pUnit->holder.read);
                writeHolder = atomic_load(&pUnit->holder.write);
                if (0xFF == readHolder || 0 != writeHolder) {
                        usleep(1 * 1000);
                        waitingTime++;
                        if (waitingTime > pQueue->timeoutWaitTime) {
                                ret |= (1 << 2);   // 操作权限申请超时
                                goto exit;
                        }
                }
                else {
                        // 获取读权限
                        atomic_fetch_add(&pUnit->holder.read, 1);
                        memcpy(pDst, pUnit->pBuffer + offset, size);
                        atomic_fetch_sub(&pUnit->holder.read, 1);
                        break;
                }
        } while (1);
exit:
        return ret;
}

/**
 * @brief 更新无锁环形队列中的最新数据
 * @param pQueue 数据同步无锁环形队列
 * @param pSrc 写入数据源
 * @param offset 拷贝偏移量
 * @param size 拷贝大小
 *
 * @return 0:正常读同步
 *         bit0:输入参数不合法
 *         bit1:队列未初始化
 *         bit2:参数内部异常
 *         bit3:操作权限申请超时
 */
uint16_t ring_lockfree_sync_queue_put(RingLockfreeSyncQueue_T* pQueue, const void* pSrc,
                                      const uint32_t offset, const uint32_t size)
{
        uint16_t ret = 0;
#ifdef RING_LOCKFREE_SYNC_QUEUE_ASSERT
        assert(NULL != pQueue);
        assert(NULL != pDst);
        assert((offset + size) <= pQueue->unitSize);
#else

        if (NULL == pQueue) {
                ret |= (1 << 0);
                goto exit;
        }
        if (NULL == pSrc) {
                ret |= (1 << 0);
                goto exit;
        }
        if ((offset + size) > pQueue->unitSize) {
                ret |= (1 << 0);
                goto exit;
        }
#endif
        const uint16_t initializeState = atomic_load(&pQueue->initializeState);
        if (0xAA55 != initializeState) {
                ret |= (1 << 1);   // 队列不支持访问
                goto exit;
        }

        // 申请写占用符
        uint8_t  readHolder  = 0;
        uint8_t  writeHolder = 0;
        uint32_t waitingTime = 0;
        do {
                uint32_t lastUnitIdx = (atomic_load(&pQueue->latestUnitIdx) + 1) % pQueue->unitNum;
                RingLockfreeUnit_T* pUnit = &pQueue->pUnits[lastUnitIdx];
                if (NULL == pUnit->pBuffer) {
                        ret |= (1 << 1);   // 队列不支持访问
                        goto exit;
                }
                readHolder  = atomic_load(&pUnit->holder.read);
                writeHolder = atomic_load(&pUnit->holder.write);
                if (0 != readHolder || 0 != writeHolder) {
                        usleep(1 * 1000);
                        waitingTime++;
                        if (waitingTime > pQueue->timeoutWaitTime) {
                                ret |= (1 << 2);   // 操作权限申请超时
                                goto exit;
                        }
                }
                else {
                        // 获取写权限
                        atomic_fetch_add(&pUnit->holder.write, 1);
                        memcpy(pUnit->pBuffer + offset, pSrc, size);
                        atomic_fetch_sub(&pUnit->holder.write, 1);
                        atomic_store(&pQueue->latestUnitIdx, lastUnitIdx);
                        break;
                }
        } while (1);
exit:
        return ret;
}
