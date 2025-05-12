/**
 * @file
 * @brief       modbus master 主机与从机数据交互函数
 * @details     主要是读同步和写同步功能
 * @author      ylb
 * @date        2025-04-25
 * @version     v1.0
 * @par History:
 *	version v1.0: 基础程序首次创建\n
 */


#include "modbus_master_api.h"

#include <modbus.h>

#include <assert.h>
#include <errno.h>


#include <string.h>

#include <math.h>
#include <time.h>

#if MODBUS_MASTER_DEBUG_PRINT
#        include <stdio.h>
#endif

/**
 * @brief 获取当前的时间戳(ms)
 *
 * @return uint64_t 当前的时间戳(ms)
 * @note 精度取决于系统
 * @par 修改日志
 *      ylb于2025-04-25创建
 */
static uint64_t get_timestamp_ms(void)
{
        struct timespec ts;
        clock_gettime(CLOCK_REALTIME, &ts);   // 使用实际时间（可受系统时间调整影响）
        const uint64_t timeStamp =
                (uint64_t)ts.tv_sec * 1000ULL + (uint64_t)ts.tv_nsec / 1000000ULL;
        return timeStamp;
}

/**
 * @brief 获取输入时间戳和当前的时间戳的差值
 * @param timestampMs 输入时间戳
 *
 * @return uint64_t 时间差(ms)
 * @note 计算结果为 当前时间戳-输入时间戳
 * @par 修改日志
 *      ylb于2025-04-25创建
 */
static int64_t get_elapsed_ms(const uint64_t timestampMs)
{
        const int64_t elapsedMs = get_timestamp_ms() - timestampMs;
        return elapsedMs;
}

/**
 * @brief 获取输入时间增加步长和当前的时间戳的和
 * @param timeStep 时间增加步长
 *
 * @return uint64_t 时间戳(ms)
 * @note 计算结果为 当前时间戳+输入时间增加步长
 * @par 修改日志
 *      ylb于2025-04-25创建
 */
static uint64_t get_future_ms(const uint64_t timeStep)
{
        const uint64_t futureMs = get_timestamp_ms() + timeStep;
        return futureMs;
}

/**
 * @brief 对uint16_t数组按照指定的单元步长进行翻转
 * @param pSrc 数据源
 * @param swapStep 单元反转步长
 * @param num 数据源的单位数量
 * @par 修改日志
 *      ylb于2025-04-25创建
 */
static void swap_groups(uint16_t* pSrc, const uint16_t swapStep, const uint16_t num)
{
        if (NULL == pSrc) {
                return;
        }
        if (0 == swapStep) {
                return;
        }
        const uint8_t u8Temp = num % swapStep;
        if (0 != u8Temp) {
                return;
        }
        // 按步长分组并逆序
        for (uint16_t group = 0; group < num; group += swapStep) {
                // 计算当前组的起始和结束索引
                uint16_t start = group;
                uint16_t end   = (group + swapStep - 1 < num) ? (group + swapStep - 1) : (num - 1);

                // 对当前组进行逆序操作
                while (start < end) {
                        const uint16_t u16temp = pSrc[start];
                        pSrc[start]            = pSrc[end];
                        pSrc[end]              = u16temp;
                        start++;
                        end--;
                }
        }
}

/**
 * 毫秒级休眠函数（自动处理信号中断）
 * @param ms 休眠毫秒数（必须 >= 0）
 * @return 0 成功，-1 失败（参数无效或不可恢复的错误）
 * @par 修改日志
 *      ylb于2025-04-25创建
 */
static int msleep(const unsigned long ms)
{
        struct timespec req, rem;
        req.tv_sec  = ms / 1000;                // 秒部分
        req.tv_nsec = (ms % 1000) * 1000000L;   // 毫秒转纳秒（确保不超过 999,999,999 ns）

        // 循环处理信号中断（EINTR）
        while (-1 == nanosleep(&req, &rem)) {
                if (EINTR != errno) return -1;   // 非信号中断错误直接返回
                req = rem;                       // 剩余时间更新为未休眠的部分
        }

        return 0;
}

/**
 * @brief 对单个可读同步信息进行读取
 * @param pCtx libmodbus 设备管理句柄
 * @param pSyncInfo 可读同步信息
 * @param deviceId 设备号
 *
 * @return 0:读取成功；
 *         bit0:pCtx为空
 *         bit1:pSyncInfo为空
 *         bit2:deviceId越界
 *         bit3:读取失败
 *         bit4:未满足读取动作启动条件
 *
 * @par 修改日志
 *      ylb于2025-04-25创建
 */
static uint8_t modbus_master_slave_register_info_read(modbus_t*                       pCtx,
                                                     ModbusMasterReadableSyncInfo_T* pSyncInfo,
                                                     const uint8_t                   deviceId)
{
#if MODBUS_MASTER_ASSERT == 1
        assert(NULL != pCtx);
        assert(NULL != pSyncInfo);
        assert(MODBUS_MASTER_MAX_SLAVE_NUM > deviceId);
#endif
        uint8_t   ret                              = 0;
        bool     readAllow                        = false;
        uint16_t u16Buffer[MODBUS_MASTER_PDU_MAX] = {0};
        if (NULL == pCtx) {
                ret |= 1 << 0;
                goto exit;
        }
        if (NULL == pSyncInfo) {
                ret |= 1 << 1;
                goto exit;
        }
        if (MODBUS_MASTER_MAX_SLAVE_NUM <= deviceId) {
                ret |= 1 << 2;
                goto exit;
        }
        // 检查可读性
        // 判断是否为周期同步，周期同步时间为0的认为是非周期读取的
        if (0 == pSyncInfo->Schedule.interval) {
                // 非周期读取的，若未读取的，允许一次读取，直到读取成功为止
                if (false == pSyncInfo->Schedule.firstRecord[deviceId]) {
                        readAllow = true;
                }
        }
        else {
                // 周期读取的，检查读取时间是否抵达，抵达则允许读取
                const int64_t timeDiffMs = get_elapsed_ms(pSyncInfo->Schedule.lastTime[deviceId]);
                if (timeDiffMs >= pSyncInfo->Schedule.interval) {
                        // 读取时间抵达，允许读取
                        readAllow = true;
                }
        }
        if (true == readAllow) {
                int reply = 0;
                switch (pSyncInfo->registerInfo.regType) {
                case MODBUS_MASTER_HOLDING_REG:
                        reply = modbus_read_registers(pCtx,
                                                      pSyncInfo->registerInfo.startAddr,
                                                      pSyncInfo->registerInfo.length,
                                                      u16Buffer);
                        break;
                case MODBUS_MASTER_INPUT_REG:
                        reply = modbus_read_input_registers(pCtx,
                                                            pSyncInfo->registerInfo.startAddr,
                                                            pSyncInfo->registerInfo.length,
                                                            u16Buffer);
                        break;
                case MODBUS_MASTER_COIL_STATUS_REG:
                        reply = modbus_read_bits(pCtx,
                                                 pSyncInfo->registerInfo.startAddr,
                                                 pSyncInfo->registerInfo.length,
                                                 (uint8_t*)u16Buffer);
                        break;
                case MODBUS_MASTER_INPUT_STATUS_REG:
                        reply = modbus_read_input_bits(pCtx,
                                                       pSyncInfo->registerInfo.startAddr,
                                                       pSyncInfo->registerInfo.length,
                                                       (uint8_t*)u16Buffer);
                        break;
                }
                if (-1 == reply) {
                        ret |= 1 << 3;
                        goto exit;
                }
                else {
                        if (0 == pSyncInfo->Schedule.interval) {
                                pSyncInfo->Schedule.firstRecord[deviceId] = true;
                        }
                        else {
                                pSyncInfo->Schedule.lastTime[deviceId] = get_timestamp_ms();
                        }
                        swap_groups(u16Buffer,
                                    pSyncInfo->registerInfo.filpStep,
                                    pSyncInfo->registerInfo.length);
                        if (NULL != pSyncInfo->ExternalApi.read_back_data_process) {
                                pSyncInfo->ExternalApi.read_back_data_process(
                                        deviceId, u16Buffer, reply);
                        }
                }
        }
        else {
                ret |= 1 << 4;
        }
exit:
        return ret;
}

/**
 * @brief 对单个可写同步信息进行写同步
 * @param pCtx libmodbus 设备管理句柄
 * @param pSyncInfo 可写同步信息
 * @param deviceId 设备号
 *
 * @return 0：同步成功
 *         bit0:pCtx为空
 *         bit1:pSyncInfo为空
 *         bit2:deviceId越界
 *         bit3:源数据获取接口为空
 *         bit4:寄存器类型不合法，不支持写入
 *         bit5:写入失败
 *         bit6:不满足写入启动条件，未能获取源数据
 *
 * @par 修改日志
 *      ylb于2025-04-25创建
 */
static uint8_t modbus_master_slave_register_info_write(
        modbus_t* pCtx, const ModbusMasterWritableSyncInfo_T* pSyncInfo, const uint8_t deviceId)
{
#if 1 == MODBUS_MASTER_ASSERT
        assert(NULL != pCtx);
        assert(NULL != pSyncInfo);
        assert(MODBUS_MASTER_MAX_SLAVE_NUM > deviceId);
        assert(NULL != pSyncInfo->ExternalApi.write_data_get);
#endif
        int8_t   ret                              = 0;
        bool     dataGetAllow                     = false;
        bool     syncAllow                        = false;
        uint16_t u16Buffer[MODBUS_MASTER_PDU_MAX] = {0};
        if (NULL == pCtx) {
                ret |= 1 << 0;
                goto exit;
        }
        if (NULL == pSyncInfo) {
                ret |= 1 << 1;
                goto exit;
        }
        if (MODBUS_MASTER_MAX_SLAVE_NUM <= deviceId) {
                ret |= 1 << 2;
                goto exit;
        }
        if (NULL == pSyncInfo->ExternalApi.write_data_get) {
                ret |= 1 << 3;
                goto exit;
        }
        if (0 != pSyncInfo->Schedule.interval) {
                // 周期写入的，检查时间是否抵达，抵达则允许读取
                const int64_t timeDiffMs = get_elapsed_ms(pSyncInfo->Schedule.lastTime[deviceId]);
                if (timeDiffMs >= pSyncInfo->Schedule.interval) {
                        // 时间抵达，允许获取数据
                        dataGetAllow = true;
                }
        }
        else {
                // 非周期写入的触发式信息，直接允许获取数据
                dataGetAllow = true;
        }
        if (true == dataGetAllow) {
                // 获取数据
                syncAllow = pSyncInfo->ExternalApi.write_data_get(deviceId, u16Buffer);
        }
        if (true == syncAllow) {
                int reply = 0;
                switch (pSyncInfo->registerInfo.regType) {
                case MODBUS_MASTER_HOLDING_REG:
                        // 对寄存器，需要执行寄存器翻转的要执行翻转
                        if (pSyncInfo->registerInfo.filpStep != MODBUS_MASTER_FILP_0) {
                                swap_groups(u16Buffer,
                                            pSyncInfo->registerInfo.filpStep,
                                            pSyncInfo->registerInfo.length);
                        }
                        // 单个寄存器的，使用06功能码写入
                        if (pSyncInfo->registerInfo.length == 1) {
                                reply = modbus_write_register(
                                        pCtx, pSyncInfo->registerInfo.startAddr, u16Buffer[0]);
                        }
                        else {
                                // 多个寄存器的，使用10功能码写入
                                reply = modbus_write_registers(pCtx,
                                                               pSyncInfo->registerInfo.startAddr,
                                                               pSyncInfo->registerInfo.length,
                                                               u16Buffer);
                        }
                        break;
                case MODBUS_MASTER_COIL_STATUS_REG:
                        if (pSyncInfo->registerInfo.length == 1) {
                                const uint8_t u8temp = *(uint8_t*)u16Buffer;
                                reply                = modbus_write_bit(
                                        pCtx, pSyncInfo->registerInfo.startAddr, u8temp);
                        }
                        else {
                                reply = modbus_write_bits(pCtx,
                                                          pSyncInfo->registerInfo.startAddr,
                                                          pSyncInfo->registerInfo.length,
                                                          (uint8_t*)u16Buffer);
                        }
                        break;
                default:
                        ret |= 1 << 4;   // 寄存器类型不合法
                        break;
                }
                if (-1 == reply) {
                        ret |= 1 << 5;
                        goto exit;
                }
        }
        else {
                ret |= 1 << 6;
        }
exit:
        return ret;
}

/**
 * @brief 设置从设备掉线
 * @param pSlaveInfo 从设备信息
 *
 * @par 修改日志
 *      ylb于2025-04-25创建
 */
static void modbus_master_set_slave_offline(ModbusMasterSlaveInfo_T* pSlaveInfo)
{
#if MODBUS_MASTER_ASSERT == 1
        assert(NULL != pSlaveInfo);
        assert(NULL != pSlaveInfo->pProtocol);
#endif
        if (NULL == pSlaveInfo) {
                return;
        }
        const ModbusMasterDataProtocol_T* pProtocol = pSlaveInfo->pProtocol;
        const int64_t timeDiffMs = get_elapsed_ms(pSlaveInfo->linkState.lastHandshakeTime);
        if (timeDiffMs >= pProtocol->time.timeoutThreshold) {
                // 失联事件超过阈值，更改设备连接状态为掉线
                pSlaveInfo->linkState.connected = false;
                // 设置下一次重试时间
                pSlaveInfo->linkState.nextHandshakeTime = get_future_ms(
                        pProtocol->time.retryInterval *
                        pow(pProtocol->time.delayCardinality, pSlaveInfo->linkState.retryCnt++));
                // 触发自定义通讯丢失接口函数
                if (NULL != pProtocol->linkApi.lose) {
                        pProtocol->linkApi.lose(pSlaveInfo->deviceId);
                }
        }
}

/**
 * @brief 设置从设备在线
 * @param pSlaveInfo 从设备信息
 *
 * @par 修改日志
 *      ylb于2025-04-25创建
 */
static void modbus_master_set_slave_online(ModbusMasterSlaveInfo_T* pSlaveInfo)
{
#if MODBUS_MASTER_ASSERT == 1
        assert(NULL != pSlaveInfo);
        assert(NULL != pSlaveInfo->pProtocol);
#endif
        if (NULL == pSlaveInfo) {
                return;
        }
        const ModbusMasterDataProtocol_T* pProtocol = pSlaveInfo->pProtocol;
        pSlaveInfo->linkState.connected             = true;
        pSlaveInfo->linkState.firstHandshakeTime    = get_timestamp_ms();
        pSlaveInfo->linkState.retryCnt              = 0;
        if (NULL != pProtocol->linkApi.connect) {
                pProtocol->linkApi.connect(pSlaveInfo->deviceId);
        }
        ModbusMasterReadableSyncInfo_T* pSyncInfo = NULL;
        for (uint16_t infoIdx = 0; infoIdx < pProtocol->readableSyncInfosNum; infoIdx++) {
                pSyncInfo = pProtocol->pReadableSyncInfos + infoIdx;
                if (NULL != pSyncInfo) {
                        if (0 == pSyncInfo->Schedule.interval) {
                                memset(pSyncInfo->Schedule.firstRecord,
                                       false,
                                       sizeof(pSyncInfo->Schedule.firstRecord));
                        }
                }
        }
}

/**
 * @brief 同步从设备所有的可读信息
 * @param pChannel 通讯通道
 * @param slaveIdx 从设备索引号
 *
 * @return ret的内容按照bit位来表示同步结果，若同步无异常，ret = 0；
 *         bit0:pCtx为空；
 *         bit1:slaveIdx越界
 *         bit2:未绑定协议表
 *         bit3:协议表不完整，缺失读取内容
 *         bit4:通道未初始化
 *         bit5:协议表内容不一致
 *         bit6:读取失败
 *
 * @par 修改日志
 *      ylb于2025-04-25创建
 */
uint8_t modbus_master_slave_readable_infos_sync(ModbusMasterChannelInfo_T* pChannel,
                                                const uint8_t              slaveIdx)
{
        uint8_t ret = 0;
#if MODBUS_MASTER_ASSERT == 1
        assert(NULL != pChannel);                                  // 通道指针不为空
        assert(pChannel->slaveNum > slaveIdx);                     // 设备索引号在设备数量范围内
        assert(NULL != pChannel->slaveInfo[slaveIdx].pProtocol);   // 设备绑定协议表不为空
        assert(NULL != pChannel->slaveInfo[slaveIdx]
                               .pProtocol->pReadableSyncInfos);   // 协议表内可读信息表不为空
#endif
        if (NULL == pChannel) {
                ret = (1 << 0);
                goto exit;
        }
        if (pChannel->slaveNum <= slaveIdx) {
                ret = (1 << 1);
                goto exit;
        }
        if (NULL == pChannel->slaveInfo[slaveIdx].pProtocol) {
                ret = (1 << 2);
                goto exit;
        }
        if (NULL == pChannel->slaveInfo[slaveIdx]
                               .pProtocol->pReadableSyncInfos) {
                ret = (1 << 3);
                goto exit;
        }
        if (false == pChannel->initialized) {
                ret &= (1 << 4);   // 通道未初始化
                goto exit;
        }
        // 检查设备轮询时间是否抵达
        ModbusMasterSlaveInfo_T*          pSlaveInfo = pChannel->slaveInfo + slaveIdx;
        const ModbusMasterDataProtocol_T* pProtocol  = pSlaveInfo->pProtocol;
        const uint8_t                     deivceId   = pSlaveInfo->deviceId;
        if (pSlaveInfo->linkState.nextHandshakeTime <= get_timestamp_ms()) {
                // 若握手时间已抵达，可以启动信息采集
                ModbusMasterReadableSyncInfo_T* pSyncInfo = NULL;
                // 设置从机地址
                modbus_set_slave(pChannel->pCtx, pSlaveInfo->slaveId);
                for (uint16_t infosIdx = 0; infosIdx < pProtocol->readableSyncInfosNum;
                     infosIdx++) {
                        pSyncInfo = pProtocol->pReadableSyncInfos + infosIdx;
                        if (NULL == pSyncInfo) {
                                ret &= (1 << 5);   // 协议表可读同步数据表及其长度不匹配
                                goto exit;
                        }
                        const int8_t feedBack = modbus_master_slave_register_info_read(
                                pChannel->pCtx, pSyncInfo, deivceId);
                        if (0 == feedBack) {
                                // 读取成功
                                if (false == pSlaveInfo->linkState.connected) {
                                        modbus_master_set_slave_online(pSlaveInfo);
                                }
                                pSlaveInfo->linkState.lastHandshakeTime = get_timestamp_ms();
                                msleep(pProtocol->time.frameInterval);   // 读取成功，按照帧间隔休眠
                        }
                        else if (-1 == feedBack) {
                                // 读取失败
#if MODBUS_MASTER_DEBUG_PRINT
                                MODBUS_MASTER_INFO_PRINT(
                                        "MODBUS MASTER:%s channel %d slave read %d data info "
                                        "failed %s\n",
                                        pChannel->channelName,
                                        slaveIdx,
                                        infosIdx,
                                        modbus_strerror(errno));
#endif
                                if (true == pSlaveInfo->linkState.connected) {
                                        modbus_master_set_slave_offline(pSlaveInfo);
                                }
                                ret &= (1 << 6);   // 读取失败
                        }
                }
        }
exit:
        return ret;
}

/**
 * @brief 同步从设备所有的可写信息
 * @param pChannel 通讯通道
 * @param slaveIdx 从设备索引号
 *
* @return ret的内容按照bit位来表示同步结果，若同步无异常，ret = 0；
 *         bit0:pCtx为空；
 *         bit1:slaveIdx越界
 *         bit2:未绑定协议表
 *         bit3:协议表不完整，缺失读取内容
 *         bit4:通道未初始化
 *         bit5:协议表内容不一致
 *         bit6:写入失败
 *
 * @par 修改日志
 *      ylb于2025-04-25创建
 */
uint8_t modbus_master_slave_writable_infos_sync(ModbusMasterChannelInfo_T* pChannel,
                                                const uint8_t              slaveIdx)
{
        uint8_t ret = 0;
#if MODBUS_MASTER_ASSERT == 1
        assert(NULL != pChannel);                                  // 通道指针不为空
        assert(pChannel->slaveNum > slaveIdx);                     // 设备索引号在设备数量范围内
        assert(NULL != pChannel->slaveInfo[slaveIdx].pProtocol);   // 设备绑定协议表不为空
        assert(NULL != pChannel->slaveInfo[slaveIdx]
                               .pProtocol->pWritableSyncInfos);   // 协议表内可读信息表不为空
#endif
        if (NULL == pChannel) {
                ret = (1 << 0);
                goto exit;
        }
        if (pChannel->slaveNum <= slaveIdx) {
                ret = (1 << 1);
                goto exit;
        }
        if (NULL == pChannel->slaveInfo[slaveIdx].pProtocol) {
                ret = (1 << 2);
                goto exit;
        }
        if (NULL == pChannel->slaveInfo[slaveIdx]
                               .pProtocol->pWritableSyncInfos) {
                ret = (1 << 3);
                goto exit;
                               }
        if (false == pChannel->initialized) {
                ret &= (1 << 4);   // 通道未初始化
                goto exit;
        }
        // 检查设备轮询时间是否抵达
        ModbusMasterSlaveInfo_T*          pSlaveInfo = pChannel->slaveInfo + slaveIdx;
        const ModbusMasterDataProtocol_T* pProtocol  = pSlaveInfo->pProtocol;
        const uint8_t                     deivceId   = pSlaveInfo->deviceId;
        if (pSlaveInfo->linkState.nextHandshakeTime <= get_timestamp_ms()) {
                // 若握手时间已抵达，可以启动信息采集
                // 设置从机地址
                modbus_set_slave(pChannel->pCtx, pSlaveInfo->slaveId);
                // 开始依次写入数据
                for (uint16_t infosIdx = 0; infosIdx < pProtocol->writableSyncInfosNum;
                     infosIdx++) {
                        const ModbusMasterWritableSyncInfo_T* pSyncInfo =
                                pProtocol->pWritableSyncInfos + infosIdx;
                        if (NULL == pSyncInfo) {
                                ret &= (1 << 5);   // 协议表可读同步数据表及其长度不匹配
                                goto exit;
                        }
                        const int8_t feedBack = modbus_master_slave_register_info_write(
                                pChannel->pCtx, pSyncInfo, deivceId);
                        if (0 == feedBack) {
                                // 写入成功
                                if (false == pSlaveInfo->linkState.connected) {
                                        modbus_master_set_slave_online(pSlaveInfo);
                                }
                                pSlaveInfo->linkState.lastHandshakeTime = get_timestamp_ms();
                                msleep(pProtocol->time.frameInterval);   // 读取成功，按照帧间隔休眠
                        }
                        else if (-1 == feedBack) {
#if MODBUS_MASTER_DEBUG_PRINT
                                MODBUS_MASTER_INFO_PRINT(
                                        "MODBUS MASTER:%s channel %d slave write %d data "
                                        "info failed %s\n",
                                        pChannel->channelName,
                                        slaveIdx,
                                        infosIdx,
                                        modbus_strerror(errno));
#endif
                                if (true == pSlaveInfo->linkState.connected) {
                                        modbus_master_set_slave_offline(pSlaveInfo);
                                }
                                ret &= (1 << 6);   // 写入失败
                        }
                }
        }
exit:
        return ret;
}
