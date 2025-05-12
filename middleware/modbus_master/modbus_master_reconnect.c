/**
* @file
 * @brief       modbus master 主机与从机连接重连函数
 * @details     基于libmodbus的close和connect实现重连操作
 * @author      ylb
 * @date        2025-04-25
 * @version     v1.0
 * @par History:
 *	version v1.0: 基础程序首次创建\n
 */

#include "modbus_master_api.h"
#include <unistd.h>

#if MODBUS_MASTER_DEBUG_PRINT == 1
#        include <stdio.h>
#endif

/**
 * @brief 通道重连
 * @param pChannel_info 通道信息
 *
 * @return 重连结果，若为true则表示成功，false表示失败
 */
bool modbus_master_channel_reconnect(ModbusMasterChannelInfo_T* pChannel_info)
{
        bool result = true;
        int  ret    = 0;
        modbus_close(pChannel_info->pCtx);
        sleep(1);
        ret = modbus_connect(pChannel_info->pCtx);
        if (ret == -1) {
                result = false;
#if MODBUS_MASTER_DEBUG_PRINT == 1
                MODBUS_MASTER_INFO_PRINT("MODBUS MASTER %s channel reconnect failed\n",
                                         pChannel_info->channelName);
#endif
        }
#if MODBUS_MASTER_DEBUG_PRINT == 1
        else {
                MODBUS_MASTER_INFO_PRINT("MODBUS MASTER %s channel reconnect successed\n",
                                         pChannel_info->channelName);
        }
#endif
        return result;
}
