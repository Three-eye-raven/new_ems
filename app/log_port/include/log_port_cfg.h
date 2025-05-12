//
// Created by Three on 2025/2/28.
//

#ifndef LOG_PORT_CFG_H
#define LOG_PORT_CFG_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include <stdatomic.h>

#define LOG_ENABLE true

#if LOG_ENABLE == true

#define LOG_LIB_ZLOG_ENABLE true

#endif

#endif //LOG_PORT_CFG_H
