//
// Created by Three on 2025/2/28.
//

#ifndef LOG_PORT_STRUCT_H
#define LOG_PORT_STRUCT_H

#include "log_port_cfg.h"

typedef struct
{
    void (*log_port_init)(void);
    void (*log_port_deinit)(void);
    void (*log_info)(const char *fmt,...);
    void (*log_debug)(const char *fmt,...);
    void (*log_warn)(const char *fmt,...);
    void (*log_error)(const char *fmt,...);
}Log_port_api_t;

#endif //LOG_PORT_STRUCT_H
