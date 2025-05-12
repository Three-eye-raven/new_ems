//
// Created by 谷子 on 2025/4/8.
//

#ifndef PVPCS_OPEN_INTERFACE_H
#define PVPCS_OPEN_INTERFACE_H

#include <stdbool.h>
#include "pvpcs_data_interaction_api.h"

bool pvpcs_rtu_communication_thread_init(void);
void pvpcs_rtu_communication_thread_join(void);

bool pvpcs_tcp_communication_thread_init(void);
void pvpcs_tcp_communication_thread_join(void);

#endif //PVPCS_OPEN_INTERFACE_H
