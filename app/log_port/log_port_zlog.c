//
// Created by Three on 2025/2/28.
//
#include "log_port_api.h"

#if LOG_LIB_ZLOG_ENABLE == true

#include "zlog.h"

#include <stdio.h>
#include <unistd.h>
#include <stdatomic.h>

zlog_category_t *info_cat = NULL;
zlog_category_t *debug_cat = NULL;
zlog_category_t *warn_cat = NULL;
zlog_category_t *error_cat = NULL;

atomic_bool zlog_info_inited = false;
atomic_bool zlog_debug_inited = false;
atomic_bool zlog_warn_inited = false;
atomic_bool zlog_error_inited = false;

static int create_or_overwrite_zlog_conf() {
    FILE *fp = fopen("zlog.conf", "w");  // "w"模式会自动覆盖已存在文件
    if (!fp) {
        perror("fopen failed");
        return -1;
    }
    const char *content =
        "[global]\n"
        "strict init = true\n"
        "buffer min = 1024\n"
        "buffer max = 2MB\n"
        "rotate lock file = /tmp/zlog.lock\n"
        "default format = \"%d.%us %-6V (%c:%F:%L) - %m%n\"\n"
        "file perms = 600\n"
        "\n"
        "[formats]\n"
        "simple = \"%d(%Y-%m-%d %H:%M:%S.%ms) [%V] %m%n\"\n"
        "\n"
        "[rules]\n"
        "info.*     \"./logs/info.%d(%F).log\", 1MB ~ \"./logs/info-%d(%F).#2r.log\";simple\n"
        "debug.*    \"./logs/debug.%d(%F).log\", 1MB ~ \"./logs/debug-%d(%F).#2r.log\"\n"
        "warning.*  \"./logs/warning.%d(%F).log\", 1MB ~ \"./logs/warning-%d(%F).#2r.log\";simple\n"
        "error.*    \"./logs/error.%d(%F).log\", 1MB ~ \"./logs/error-%d(%F).#2r.log\";simple\n";

    if (fputs(content, fp) == EOF) {
        perror("zlog.conf write failed");
        fclose(fp);
        return -1;
    }

    if (fclose(fp) != 0) {
        perror("zlog.conf close failed");
        return -1;
    }
    return 0;
}

static void zlog_info_category_init(void) {
    info_cat = zlog_get_category("info");
    if (!info_cat)
    {
        printf("zlog init category get fail\n");
    }
    else
    {
        atomic_store(&zlog_info_inited, true);
    }
}

static void zlog_debug_category_init(void) {
    debug_cat = zlog_get_category("debug");
    if (!debug_cat)
    {
        printf("zlog debug category get fail\n");
    }
    else
    {
        atomic_store(&zlog_debug_inited, true);
    }
}

static void zlog_warning_category_init(void) {
    warn_cat = zlog_get_category("warning");
    if (!warn_cat)
    {
        printf("zlog warning category get fail\n");
    }
    else
    {
        atomic_store(&zlog_warn_inited, true);
    }
}

static void zlog_error_category_init(void) {
    error_cat = zlog_get_category("error");
    if (!error_cat)
    {
        printf("zlog error category get fail\n");
    }
    else
    {
        atomic_store(&zlog_error_inited, true);
    }
}

void log_port_zlog_init(void)
{
    int ret = 0;
    ret = create_or_overwrite_zlog_conf();
    if (ret)
    {
        printf("create and overwrite zlog conf failed\n");
        return;
    }
    ret = zlog_init("zlog.conf");
    if (ret)
    {
        printf("zlog init fail\n");
        return;
    }
    zlog_info_category_init();
    zlog_debug_category_init();
    zlog_warning_category_init();
    zlog_error_category_init();
}

void log_port_zlog_deinit(void)
{
    atomic_store(&zlog_info_inited, false);
    atomic_store(&zlog_debug_inited, false);
    atomic_store(&zlog_warn_inited, false);
    atomic_store(&zlog_error_inited, false);
    zlog_fini();
}

#endif
