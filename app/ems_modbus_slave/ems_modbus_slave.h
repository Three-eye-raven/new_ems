//
// Created by three on 2025/5/7.
//

#ifndef EMS_MODBUS_SLAVE_H
#define EMS_MODBUS_SLAVE_H

#include <stdint.h>
#include <stdbool.h>

uint16_t emsHolderRegisterSyncListInit(void);
void emsHolderRegisterSyncListDeinit(void);

uint16_t ems_modbus_slave_sys_firmware_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size);
uint16_t ems_modbus_slave_sys_run_info_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size);
uint16_t emd_modbus_slave_bcu_register_update(const uint8_t bcuId,const void* pSrc, const uint32_t offset,
                                               const uint32_t size);
uint16_t emd_modbus_slave_pcs_register_update(const uint8_t pcsId,const void* pSrc, const uint32_t offset,
                                               const uint32_t size);
uint16_t ems_modbus_slave_internal_meter_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size);
uint16_t ems_modbus_slave_external_meter_register_update(const void* pSrc, const uint32_t offset,
                                               const uint32_t size);
uint16_t emd_modbus_slave_pvpcs_register_update(const uint8_t pvpcsId,const void* pSrc, const uint32_t offset,
                                               const uint32_t size);

bool ems_modbus_slave_thread_init(void);
void ems_modbus_slave_thread_join(void);


#endif //EMS_MODBUS_SLAVE_H
