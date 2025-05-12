/**
* @file
 * @brief       modbus master tcp通道初始化函数
 * @details     包含ipv4地址的合法性检查，端口号检查以及libmodbus句柄申请
 * @author      ylb
 * @date        2025-04-25
 * @version     v1.0
 * @par History:
 *	version v1.0: 基础程序首次创建\n
 */

#include "modbus.h"
#include "modbus_master_api.h"

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 判断ipv4地址字符串是否合法
 * @param pIp ipv4地址
 *
 * @return 合法则返回true,不合法返回false
 */
static bool is_valid_ipv4(const char* pIp)
{
        if (pIp == NULL || *pIp == '\0') return false;
        int                     num_count   = 0;   // 数字段计数器
        int                     current_num = 0;   // 当前处理的数字
        int                     segment_len = 0;   // 当前段的长度
        const long unsigned int len         = strlen(pIp);
        for (int i = 0; i <= len; i++) {
                const char ch = pIp[i];
                if (isdigit(ch) && ch != '\0') {
                        // 处理数字字符
                        current_num = current_num * 10 + (ch - '0');
                        segment_len++;
                        // 检查数字段长度是否超过3位
                        if (segment_len > 3) return false;
                }
                else if (ch == '.' || ch == '\0') {
                        // 完成一个段的验证
                        num_count++;
                        // 验证规则：
                        // 1. 段长度不能为0（空段）
                        // 2. 数字值不能超过255
                        // 3. 禁止前导零（除非是单个0）
                        if (segment_len == 0 || current_num > 255 ||
                            (segment_len > 1 && pIp[i - segment_len] == '0')) {
                                return false;
                        }
                        // 重置计数器
                        current_num = 0;
                        segment_len = 0;
                        // 提前终止检查（最多4个段）
                        if (num_count > 4) return false;
                }
                else {
                        // 非法字符
                        return false;
                }
        }
        // 必须正好有4个数字段
        return num_count == 4;
}

/**
 * @brief 判断端口号是否合法
 * @param port 端口号
 *
 * @return 合法则返回true,不合法返回false
 */
static bool is_valid_port(const int port)
{
        return (port > 0 && port <= 65535);
}

/**
 * @brief 判断ip配置是否合法
 * @param pConfig ip配置
 * @param pChannelName 通道名称
 *
 * @return 返回0表示检查通过，配置信息合法；
 *         bit0置位表示配置文件信息指针为空
 *         bit1置位表示ip地址不合法
 *         bit2置位表示端口号不合法
 */
static uint8_t modbus_master_tcp_channel_config_check(const ModbusMasterTcpInfo_T* pConfig,
                                                      char*                        pChannelName)
{
        uint8_t ret = 0;
        if (pConfig == NULL) {
                ret |= 1 << 0;
#if MODBUS_MASTER_DEBUG_PRINT == 1
                if (pChannelName != NULL) {
                        MODBUS_MASTER_INFO_PRINT("MODBUS MASTER:%s 配置指针异常\n", pChannelName);
                }
#endif
                return ret;
        }
        if (is_valid_ipv4(pConfig->ipv4) == false) {
                ret |= 1 << 1;
#if MODBUS_MASTER_DEBUG_PRINT == 1
                if (pChannelName != NULL) {
                        MODBUS_MASTER_INFO_PRINT("MODBUS MASTER:%s ipv4地址填写错误\n",
                                                 pChannelName);
                }
#endif
        }
        if (is_valid_port(pConfig->port) == false) {
                ret |= 1 << 2;
#if MODBUS_MASTER_DEBUG_PRINT == 1
                if (pChannelName != NULL) {
                        MODBUS_MASTER_INFO_PRINT("MODBUS MASTER:%s 端口号填写错误\n", pChannelName);
                }
#endif
        }
        return ret;
}

/**
 * @brief 初始化
 * @param pChannel Tcp 类型通道
 *
 * @return 初始化结果，若为true则表示成功，false表示失败
 */
bool modbus_master_tcp_channel_init(ModbusMasterTcpChannel_T* pChannel)
{
        bool ret = true;
        if (pChannel == NULL) {
#if MODBUS_MASTER_DEBUG_PRINT == 1
                MODBUS_MASTER_INFO_PRINT("MODBUS MASTER:初始化时输入了空指针\n");
#endif
                ret = false;
                goto exit;
        }
        const ModbusMasterTcpInfo_T* pConfig      = &pChannel->config;
        ModbusMasterChannelInfo_T*   pChannelInfo = &pChannel->channelInfo;
        // 参数检查部分
        if (modbus_master_tcp_channel_config_check(pConfig, pChannelInfo->channelName) != 0) {
                ret = false;
#if MODBUS_MASTER_DEBUG_PRINT == 1
                MODBUS_MASTER_INFO_PRINT("MODBUS MASTER:%s 通道参数异常，请根据提示进行检查\n",
                                         pChannelInfo->channelName);
#endif
                goto exit;
        }
        // 初始化执行
        pChannelInfo->pCtx = modbus_new_tcp(pConfig->ipv4, pConfig->port);
        // 初始化成功
        if (pChannelInfo->pCtx != NULL) {
                if (modbus_connect(pChannelInfo->pCtx) == -1) {
#if MODBUS_MASTER_DEBUG_PRINT == 1
                        MODBUS_MASTER_INFO_PRINT("MODBUS MASTER:%s 连接失败: %s\n",
                                                 pChannelInfo->channelName,
                                                 modbus_strerror(errno));
#endif
                }
                pChannelInfo->initialized = true;
        }
        // 初始化失败
        else {
#if MODBUS_MASTER_DEBUG_PRINT == 1
                MODBUS_MASTER_INFO_PRINT("MODBUS MASTER:%s 句柄申请失败\n",
                                         pChannelInfo->channelName);
#endif
                ret = false;
        }
        if (ret == false) {
                pChannelInfo->initialized = false;
        }
exit:
        return ret;
}

/**
 * @brief 反初始化
 * @param pChannel Tcp 类型通道
 *
 */
void modbus_master_tcp_channel_deinit(ModbusMasterTcpChannel_T* pChannel)
{
        if (pChannel == NULL) {
                return;
        }
        if (pChannel->channelInfo.initialized == false) {
                return;
        }
        else {
                if (pChannel->channelInfo.pCtx != NULL) {
                        modbus_close(pChannel->channelInfo.pCtx);
                        modbus_free(pChannel->channelInfo.pCtx);
                        pChannel->channelInfo.pCtx = NULL;
                }
        }
}
