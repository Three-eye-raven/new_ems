//
// Created by three on 2025/4/25.
//

#ifndef PVPCS_HW_110KW_LINK_API_H
#define PVPCS_HW_110KW_LINK_API_H

#include <stdint.h>

void pvpcs_hw_110kw_link_lose(uint16_t deviceId);
void pvpcs_hw_110kw_link_online(uint16_t deviceId,uint32_t timeStep);
void pvpcs_hw_110kw_link_connect(uint16_t deviceId);

#endif //PVPCS_HW_110KW_LINK_API_H
