//
// Created by three on 2025/4/25.
//
#include "pvpcs_hw_110kw_link_api.h"
#include "pvpcs_data_interaction_api.h"

void pvpcs_hw_110kw_link_lose(const uint16_t deviceId)
{
        const uint32_t connectStatus = pvpcs_connect_status_get() & (~(1 << deviceId));
        pvpcs_connect_state_set(deviceId, false);
        pvpcs_connect_status_set(connectStatus);
}

void pvpcs_hw_110kw_link_online(uint16_t deviceId, uint32_t timeStep) {}

void pvpcs_hw_110kw_link_connect(const uint16_t deviceId)
{
        const uint32_t connectStatus = pvpcs_connect_status_get() | (1 << deviceId);
        pvpcs_connect_state_set(deviceId, true);
        pvpcs_connect_status_set(connectStatus);
}
