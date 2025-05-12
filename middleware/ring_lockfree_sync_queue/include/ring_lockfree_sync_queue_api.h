//
// Created by three on 2025/5/4.
//

#ifndef RING_LOCKFREE_SYNC_QUEUE_API_H
#define RING_LOCKFREE_SYNC_QUEUE_API_H

#include "ring_lockfree_sync_queue_struct.h"

uint16_t                 ring_lockfree_sync_queue_init(RingLockfreeSyncQueue_T* pQueue);
void                     ring_lockfree_sync_queue_deinit(RingLockfreeSyncQueue_T* pQueue);
RingLockfreeSyncQueue_T* ring_lockfree_sync_queue_new(const uint32_t unitSize,
                                                      const uint32_t unitNum,
                                                      const uint32_t timeoutWaitTime);
void                     ring_lockfree_sync_queue_free(RingLockfreeSyncQueue_T* pQueue);

uint16_t ring_lockfree_sync_queue_get(RingLockfreeSyncQueue_T* pQueue, void* pDst,
                                      const uint32_t offset, const uint32_t size);

uint16_t ring_lockfree_sync_queue_put(RingLockfreeSyncQueue_T* pQueue, const void* pSrc,
                                      const uint32_t offset, const uint32_t size);

void ring_lockfree_sync_queue_test(void);

#endif   // RING_LOCKFREE_SYNC_QUEUE_API_H
