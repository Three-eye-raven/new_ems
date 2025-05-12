/**
* @file
 * @brief       modbus master rtu 通道初始化
 * @details     包含设备路径检查和串口配置参数检查，libmodbus句柄申请
 * @author      ylb
 * @date        2025-04-25
 * @version     v1.0
 * @par History:
 *	version v1.0: 基础程序首次创建\n
 */

#include "modbus.h"
#include "modbus_master_api.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

/**
 * @brief 在linux系统中，判断路径是否存在
 * @param pPath 绝对路径
 *
 * @return 路径存在返回true,不存在返回false
 */
static bool path_exists(const char* pPath)
{
        struct stat statbuf;
        return stat(pPath, &statbuf) == 0;
}

/**
 * @brief 判断串口配置的奇偶校验位是否合法
 * @param parity 奇偶校验位
 *
 * @return 合法则返回true,不合法则返回false
 */
static bool parity_check(const char parity)
{
        bool ret = true;
        if (parity != 'N' && parity != 'E' && parity != 'O') {
                ret = false;
        }
        return ret;
}

/**
 * @brief 判断串口配置是否合法
 * @param pConfig 串口配置
 * @param pChannelName 通道名称
 *
 * @return 返回0表示检查通过，配置信息合法；
 *         bit0置位表示配置文件信息指针为空
 *         bit1置位表示路径不存在
 *         bit2置位表示奇偶校验位错误
 */
static uint8_t modbus_master_rtu_channel_serial_info_check(ModbusMasterSerialInfo_T* pConfig,
                                                           char*                     pChannelName)
{
        uint8_t ret = 0;
        if (pConfig == NULL) {
                ret |= 1 << 0;
#if MODBUS_MASTER_DEBUG_PRINT == 1
                if (pChannelName != NULL) {
                        MODBUS_MASTER_INFO_PRINT("MODBUS MASTER:%s 配置文件指针异常\n",
                                                 pChannelName);
                }
#endif
                return ret;
        }
        if (path_exists(pConfig->serialName) == false) {
                ret |= 1 << 1;
#if MODBUS_MASTER_DEBUG_PRINT == 1
                if (pChannelName != NULL) {
                        MODBUS_MASTER_INFO_PRINT("MODBUS MASTER:%s 设备物理句柄 %s 不存在\n",
                                                 pChannelName,
                                                 pConfig->serialName);
                }
#endif
        }
        if (parity_check(pConfig->parity) == false) {
                ret |= 1 << 2;
#if MODBUS_MASTER_DEBUG_PRINT == 1
                if (pChannelName != NULL) {
                        MODBUS_MASTER_INFO_PRINT("MODBUS MASTER:%s 奇偶校验码不正确 异常值为%c\n",
                                                 pChannelName,
                                                 pConfig->parity);
                }
#endif
        }
        return ret;
}

/**
 * @brief 初始化
 * @param pChannel Rtu 类型通道
 *
 * @return 初始化结果，若为true则表示成功，false表示失败
 */
bool modbus_master_rtu_channel_init(ModbusMasterRtuChannel_T* pChannel)
{
        bool ret = true;
        if (pChannel == NULL) {
#if MODBUS_MASTER_DEBUG_PRINT == 1
                MODBUS_MASTER_INFO_PRINT("MODBUS MASTER:初始化时输入了空指针\n");
#endif
                goto exit;
        }
        ModbusMasterSerialInfo_T*  pConfig      = &pChannel->config;
        ModbusMasterChannelInfo_T* pChannelInfo = &pChannel->channelInfo;
        // 参数检查部分
        if (modbus_master_rtu_channel_serial_info_check(pConfig, pChannelInfo->channelName) != 0) {
                ret = false;
#if MODBUS_MASTER_DEBUG_PRINT == 1
                MODBUS_MASTER_INFO_PRINT("MODBUS MASTER:%s 通道参数异常，请根据提示进行检查\n",
                                         pChannelInfo->channelName);
#endif
                goto exit;
        }
        // 初始化执行
        pChannelInfo->pCtx = modbus_new_rtu(pConfig->serialName,
                              pConfig->baud,
                              pConfig->parity,
                              pConfig->dataBit,
                              pConfig->stopBit);
        // 初始化成功
        if (pChannelInfo->pCtx != NULL) {
                if (modbus_connect(pChannelInfo->pCtx) == -1) {
#if MODBUS_MASTER_DEBUG_PRINT == 1
                        MODBUS_MASTER_INFO_PRINT("MODBUS MASTER:%s 连接失败: %s\n",
                                                 pChannelInfo->channelName,
                                                 modbus_strerror(errno));
#endif
                }
                else {
                        pChannelInfo->initialized = true;
                }
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
 * @param pChannel Rtu 类型通道
 *
 */
void modbus_master_rtu_channel_deinit(ModbusMasterRtuChannel_T* pChannel)
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