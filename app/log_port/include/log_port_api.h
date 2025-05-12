//
// Created by Three on 2025/2/28.
//

#ifndef LOG_PORT_API_H
#define LOG_PORT_API_H

#include "log_port_struct.h"

#if LOG_LIB_ZLOG_ENABLE
#include "zlog.h"
void log_port_zlog_init(void);
void log_port_zlog_deinit(void);

extern zlog_category_t *info_cat;
extern zlog_category_t *debug_cat;
extern zlog_category_t *warn_cat;
extern zlog_category_t *error_cat;

extern atomic_bool zlog_info_inited;
extern atomic_bool zlog_debug_inited;
extern atomic_bool zlog_warn_inited;
extern atomic_bool zlog_error_inited;

#define LOG_INIT() log_port_zlog_init()
#define LOG_DEINIT() log_port_zlog_deinit()
#define LOG_INFO(fmt, ...) if(zlog_info_inited){zlog_info(info_cat, fmt, ##__VA_ARGS__);}
#define LOG_DEBUG(fmt,...) if(zlog_debug_inited){zlog_debug(debug_cat, fmt, ##__VA_ARGS__);}
#define LOG_WARN(fmt,...) if(zlog_error_inited){zlog_error(error_cat, fmt, ##__VA_ARGS__);}
#define LOG_ERROR(fmt,...) if(zlog_warning_inited){zlog_warn(fatal_cat, fmt, ##__VA_ARGS__);}

#else
#define LOG_INIT()
#define LOG_DEINIT()
#define LOG_INFO(fmt, ...)
#define LOG_DEBUG(fmt,...)
#define LOG_WARN(fmt,...)
#define LOG_ERROR(fmt,...)

#endif

#endif //LOG_PORT_API_H
