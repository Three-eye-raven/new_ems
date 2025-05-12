//
// Created by three on 2025/5/7.
//

#include "ems_modbus_slave.h"

#include "modbus_slave.h"
#include "modbus_slave_sync_list.h"

#include "ring_lockfree_sync_queue_api.h"

#define EMS_MODBUS_SLAVE_INTERVAL_NUM 19

typedef enum
{
        EMS_SYS_FIRMWARE_INFO,   // 0 ,0x100
        ENS_SYS_RUN_INFO,        // 256, 0xf00
        EMS_BCU1,                // 0x1000,0x1000
        EMS_BCU2,                // 0x2000
        EMS_BCU3,                // 0x3000
        EMS_BCU4,                // 0x4000
        EMS_BCU5,                // 0x5000
        EMS_PCS1,                // 0x6000,0x100
        EMS_PCS2,                // 0x6100
        EMS_PCS3,                // 0x6200
        EMS_PCS4,                // 0x6300
        EMS_PCS5,                // 0x6400
        EMS_INTERNAL_METER,      // 0x6500,0x100
        EMS_EXTERNAL_METER,      // 0x6600,0x100
        EMS_PVPCS1,              // 0x6700,0x100
        EMS_PVPCS2,              // 0x6800
        EMS_PVPCS3,              // 0x6900
        EMS_PVPCS4,              // 0x6A00
        EMS_PVPCS5,              // 0x6B00
} EMS_POINTS_TABLE_E;

static ModbusSlaveAddressInterval_T holderResgisterInterval[EMS_MODBUS_SLAVE_INTERVAL_NUM] = {
        [EMS_SYS_FIRMWARE_INFO] =
                {
                        .start  = 0,
                        .length = 0x100,
                },
        [ENS_SYS_RUN_INFO] =
                {
                        .start  = 0x100,
                        .length = 0xF00,
                },
        [EMS_BCU1] =
                {
                        .start  = 0x1000,
                        .length = 0x1000,
                },
        [EMS_BCU2] =
                {
                        .start  = 0x2000,
                        .length = 0x1000,
                },
        [EMS_BCU3] =
                {
                        .start  = 0x3000,
                        .length = 0x1000,
                },
        [EMS_BCU4] =
                {
                        .start  = 0x4000,
                        .length = 0x1000,
                },
        [EMS_BCU5] =
                {
                        .start  = 0x5000,
                        .length = 0x1000,
                },
        [EMS_PCS1] =
                {
                        .start  = 0x6000,
                        .length = 0x100,
                },
        [EMS_PCS2] =
                {
                        .start  = 0x6100,
                        .length = 0x100,
                },
        [EMS_PCS3] =
                {
                        .start  = 0x6200,
                        .length = 0x100,
                },
        [EMS_PCS4] =
                {
                        .start  = 0x6300,
                        .length = 0x100,
                },
        [EMS_PCS5] =
                {
                        .start  = 0x6400,
                        .length = 0x100,
                },
        [EMS_INTERNAL_METER] =
                {
                        .start  = 0x6500,
                        .length = 0x100,
                },
        [EMS_EXTERNAL_METER] =
                {
                        .start  = 0x6600,
                        .length = 0x100,
                },
        [EMS_PVPCS1] =
                {
                        .start  = 0x6700,
                        .length = 0x100,
                },
        [EMS_PVPCS2] =
                {
                        .start  = 0x6800,
                        .length = 0x100,
                },
        [EMS_PVPCS3] =
                {
                        .start  = 0x6900,
                        .length = 0x100,
                },
        [EMS_PVPCS4] =
                {
                        .start  = 0x6A00,
                        .length = 0x100,
                },
        [EMS_PVPCS5] =
                {
                        .start  = 0x6B00,
                        .length = 0x100,
                },
};

static RingLockfreeSyncQueue_T holderResgisterQueue[EMS_MODBUS_SLAVE_INTERVAL_NUM] = {
        [EMS_SYS_FIRMWARE_INFO] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
        [ENS_SYS_RUN_INFO] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
        [EMS_BCU1] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
        [EMS_BCU2] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
        [EMS_BCU3] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
        [EMS_BCU4] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
        [EMS_BCU5] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
        [EMS_PCS1] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
        [EMS_PCS2] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
        [EMS_PCS3] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
        [EMS_PCS4] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
        [EMS_PCS5] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
        [EMS_INTERNAL_METER] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
        [EMS_EXTERNAL_METER] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
        [EMS_PVPCS1] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
        [EMS_PVPCS2] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
        [EMS_PVPCS3] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
        [EMS_PVPCS4] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
        [EMS_PVPCS5] =
                {
                        .initializeState = 0,
                        .latestUnitIdx   = 0,
                        .unitSize        = 0,
                        .unitNum         = MODBUS_SLAVE_SYNC_LIST_DEFAULT_UNITNUM,
                        .timeoutWaitTime = MODBUS_SLAVE_SYNC_LIST_DEFAULT_TIMEOUT_WAIT_TIME,
                        .pUnits          = NULL,
                },
};

ModbusSlaveSyncList_T emsHolderRegisterSyncList = {
        .initializedState = false,
        .cellSize         = sizeof(uint16_t),
        .intervalNum      = EMS_MODBUS_SLAVE_INTERVAL_NUM,
        .pIntervals       = holderResgisterInterval,
        .pQueue           = holderResgisterQueue,
};

uint16_t emsHolderRegisterSyncListInit(void)
{
        const uint16_t ret = modbus_slave_sync_list_init(&emsHolderRegisterSyncList);
        return ret;
}

void emsHolderRegisterSyncListDeinit(void)
{
        modbus_slave_sync_list_deinit(&emsHolderRegisterSyncList);
}

static uint16_t ems_modbus_slave_holder_register_update(const EMS_POINTS_TABLE_E pointTable,
                                                        const void* pSrc, const uint32_t offset,
                                                        const uint32_t size)
{
        uint16_t ret = 0;
        if (NULL == pSrc) {
                ret |= (1 << 0);
                goto exit;
        }
        if (((offset + size) * sizeof(uint16_t)) > holderResgisterQueue[pointTable].unitSize) {
                ret |= (1 << 0);   // 不支持跨区域写入
                goto exit;
        }
        const uint16_t reply =
                ring_lockfree_sync_queue_put(&holderResgisterQueue[pointTable], pSrc, offset, size);
        if (0 != reply) {
                ret |= (1 << 1);   // 写入失败
        }
exit:
        return ret;
}

typedef struct
{
        uint16_t (*updata)(const void* pSrc, const uint32_t offset,
                                               const uint32_t size);
}EmsModbusSlaveRegisterUpdateApi_T;

uint16_t ems_modbus_slave_sys_firmware_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(EMS_SYS_FIRMWARE_INFO, pSrc, offset, size);
        return ret;
}

uint16_t ems_modbus_slave_sys_run_info_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(ENS_SYS_RUN_INFO, pSrc, offset, size);
        return ret;
}

static uint16_t ems_modbus_slave_bcu1_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(EMS_BCU1, pSrc, offset, size);
        return ret;
}

static uint16_t ems_modbus_slave_bcu2_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(EMS_BCU2, pSrc, offset, size);
        return ret;
}

static uint16_t ems_modbus_slave_bcu3_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(EMS_BCU3, pSrc, offset, size);
        return ret;
}

static uint16_t ems_modbus_slave_bcu4_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(EMS_BCU4, pSrc, offset, size);
        return ret;
}

static uint16_t ems_modbus_slave_bcu5_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(EMS_BCU5, pSrc, offset, size);
        return ret;
}

static uint16_t ems_modbus_slave_pcs1_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(EMS_PCS1, pSrc, offset, size);
        return ret;
}

static uint16_t ems_modbus_slave_pcs2_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(EMS_PCS2, pSrc, offset, size);
        return ret;
}

static uint16_t ems_modbus_slave_pcs3_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(EMS_PCS3, pSrc, offset, size);
        return ret;
}

static uint16_t ems_modbus_slave_pcs4_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(EMS_PCS4, pSrc, offset, size);
        return ret;
}

static uint16_t ems_modbus_slave_pcs5_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(EMS_PCS5, pSrc, offset, size);
        return ret;
}

uint16_t ems_modbus_slave_internal_meter_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(EMS_INTERNAL_METER, pSrc, offset, size);
        return ret;
}

uint16_t ems_modbus_slave_external_meter_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(EMS_EXTERNAL_METER, pSrc, offset, size);
        return ret;
}

static uint16_t ems_modbus_slave_pvpcs1_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(EMS_PVPCS1, pSrc, offset, size);
        return ret;
}

static uint16_t ems_modbus_slave_pvpcs2_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(EMS_PVPCS2, pSrc, offset, size);
        return ret;
}

static uint16_t ems_modbus_slave_pvpcs3_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(EMS_PVPCS3, pSrc, offset, size);
        return ret;
}

static uint16_t ems_modbus_slave_pvpcs4_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(EMS_PVPCS4, pSrc, offset, size);
        return ret;
}

static uint16_t ems_modbus_slave_pvpcs5_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        const uint16_t ret = ems_modbus_slave_holder_register_update(EMS_PVPCS5, pSrc, offset, size);
        return ret;
}

const static EmsModbusSlaveRegisterUpdateApi_T emsBcuUpdate[5] = {
        ems_modbus_slave_bcu1_register_update,
        ems_modbus_slave_bcu2_register_update,
        ems_modbus_slave_bcu3_register_update,
        ems_modbus_slave_bcu4_register_update,
        ems_modbus_slave_bcu5_register_update,
};

uint16_t emd_modbus_slave_bcu_register_update(const uint8_t bcuId,const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        uint16_t ret = 0;
        if (bcuId < 5) {
                ret = emsBcuUpdate[bcuId].updata(pSrc, offset, size);
        }
        return ret;
}

const static EmsModbusSlaveRegisterUpdateApi_T emsPcsUpdate[5] = {
        ems_modbus_slave_pcs1_register_update,
        ems_modbus_slave_pcs2_register_update,
        ems_modbus_slave_pcs3_register_update,
        ems_modbus_slave_pcs4_register_update,
        ems_modbus_slave_pcs5_register_update,
};

uint16_t emd_modbus_slave_pcs_register_update(const uint8_t pcsId,const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        uint16_t ret = 0;
        if (pcsId < 5) {
                ret = emsPcsUpdate[pcsId].updata(pSrc, offset, size);
        }
        return ret;
}

const static EmsModbusSlaveRegisterUpdateApi_T emsPvpcsUpdate[5] = {
        ems_modbus_slave_pvpcs1_register_update,
        ems_modbus_slave_pvpcs2_register_update,
        ems_modbus_slave_pvpcs3_register_update,
        ems_modbus_slave_pvpcs4_register_update,
        ems_modbus_slave_pvpcs5_register_update,
};

uint16_t emd_modbus_slave_pvpcs_register_update(const uint8_t pvpcsId,const void* pSrc, const uint32_t offset,
                                               const uint32_t size)
{
        uint16_t ret = 0;
        if (pvpcsId < 5) {
                ret = emsPvpcsUpdate[pvpcsId].updata(pSrc, offset, size);
        }
        return ret;
}