//
// Created by 谷子 on 2025/4/8.
//

#ifndef PVPCS_CFG_H
#define PVPCS_CFG_H

typedef enum
{
    SUNGROW_PVPCS,
    HW_PVPCS_110KW,
    HW_PVPCS_150KW,
}pvpcs_protocols_name_e;

#define PVPCS_PROTOCOLS_NUM (HW_PVPCS_150KW + 1)

#endif //PVPCS_CFG_H
