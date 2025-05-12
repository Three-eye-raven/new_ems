/**
 * @file
 * @brief       modbus master 数据结构
 * @author      ylb
 * @date        2025-04-25
 * @version     v1.0
 * @par History:
 *	version v1.0: 基础程序首次创建\n
 */

#ifndef MODBUS_MASTER_STRUCT_H
#define MODBUS_MASTER_STRUCT_H

#include "modbus_master_cfg.h"

#include "modbus.h"
#include <stdbool.h>
#include <stdint.h>

/**
 * @brief modbus master 寄存器信息结构体
 */
typedef struct
{
        uint16_t                      startAddr; /*!< 起始地址，范围0-65535，从变量类型上做限制 */
        uint8_t                       length;    /*!< 寄存器长度*/
        MODBUS_MASTER_REG_TYPE_E      regType;   /*!< 寄存器类型，枚举类型，限制错误填写 */
        MODBUS_MASTER_REG_FILP_STEP_E filpStep;  /*!< 反转步长，枚举类型，限制错误填写 */
} ModbusMasterRegisterInfo_T;

/**
 * @brief 可读同步信息
 */
typedef struct
{
        ModbusMasterRegisterInfo_T registerInfo; /*!< 寄存器信息 */
        struct
        {
                uint32_t interval;                                 /*!< 同步间隔(ms) */
                uint64_t lastTime[MODBUS_MASTER_MAX_SLAVE_NUM];    /*!< 上次同步时间戳 */
                bool     firstRecord[MODBUS_MASTER_MAX_SLAVE_NUM]; /*!< 首次同步记录 */
        } Schedule;
        struct
        {
                void (*read_back_data_process)(
                        uint16_t deviceId, void* pRawData,
                        uint8_t dataSize); /*!< 数据读取成功后数据处理函数接口 */
        } ExternalApi;
} ModbusMasterReadableSyncInfo_T;

/**
 * @brief 可写同步信息
 */
typedef struct
{
        ModbusMasterRegisterInfo_T registerInfo; /*!< 寄存器信息 */
        struct
        {
                uint32_t interval;                              /*!< 同步时间间隔 */
                uint64_t lastTime[MODBUS_MASTER_MAX_SLAVE_NUM]; /*!< 上次同步时间 */
        } Schedule;
        struct
        {
                bool (*write_data_get)(
                        uint16_t deviceId,
                        void*    pRawData); /*!<
                                            源数据获取接口,接口要求若检查数据更新的话，未更新则返回数据获取失败，更新则返回数据获取成功
                                          */
        } ExternalApi;
} ModbusMasterWritableSyncInfo_T;

/**
 * @brief 协议时间参数配置
 */
typedef struct
{
        uint16_t frameInterval;    /*!< 帧间隔(ms) */
        uint16_t retryInterval;    /*!< 重试间隔（ms) */
        uint16_t delayCardinality; /*!< 延迟基数 */
        uint32_t timeoutThreshold; /*!< 超时阈值(ms) */
} ModbusMasterProtocolTime_T;

/**
 * @brief 协议连接动作api接口
 */
typedef struct
{
        void (*lose)(uint16_t deviceId); /*!< 设备掉线时触发的回调函数 */
        void (*online)(uint16_t deviceId,
                       uint32_t timeStep);  /*!< 设备在线时触发的回调函数，周期执行 */
        void (*connect)(uint16_t deviceId); /*!< 设备上线时触发的回调函数 */
} ModbusMasterProtocolLinkApi_T;

/**
 * @brief 协议表
 */
typedef struct
{
        char                            protocolsName[32];    /*!< 协议名称 */
        ModbusMasterReadableSyncInfo_T* pReadableSyncInfos;   /*!< 可读同步信息表 */
        uint16_t                        readableSyncInfosNum; /*!< 可读同步信息表长度 */
        ModbusMasterWritableSyncInfo_T* pWritableSyncInfos;   /*!< 可写同步信息表 */
        uint16_t                        writableSyncInfosNum; /*!< 可写同步信息表长度 */
        ModbusMasterProtocolTime_T      time;                 /*!< 协议时间参数配置 */
        ModbusMasterProtocolLinkApi_T   linkApi;              /*!< 协议连接动作api接口 */
} ModbusMasterDataProtocol_T;

/**
 * @brief 设备连接状态
 */
typedef struct
{
        bool     connected;          /*!< 初值为false */
        uint64_t firstHandshakeTime; /*!< 首次握手时间，重连后刷新 */
        uint64_t lastHandshakeTime;  /*!< 最后一次握手时间，每次握手成功刷新 */
        uint64_t nextHandshakeTime;  /*!< 下次尝试握手时间 */
        uint64_t retryCnt;           /*!< 重试次数记录 */
} ModbusMasterDeviceLinkState_T;

/**
 * @brief 从设备信息
 */
typedef struct
{
        uint8_t                       deviceId;  /*!< 设备号 */
        uint8_t                       slaveId;   /*!< 从机id */
        ModbusMasterDeviceLinkState_T linkState; /*!< 连接状态 */
        ModbusMasterDataProtocol_T*   pProtocol; /*!< 协议表 */
} ModbusMasterSlaveInfo_T;

/**
 * @brief 通道信息
 */
typedef struct
{
        char                    channelName[32];                        /*!< 通道名称 */
        bool                    initialized;                            /*!< 初始化完成标志 */
        uint8_t                 slaveNum;                               /*!< 从设备数量 */
        ModbusMasterSlaveInfo_T slaveInfo[MODBUS_MASTER_MAX_SLAVE_NUM]; /*!< 从设备信息表 */
        modbus_t*               pCtx;                                   /*!< 物理接口句柄 */
} ModbusMasterChannelInfo_T;

/**
 * @brief 串口配置
 */
typedef struct
{
        char                            serialName[32]; /*!< 设备路径名 */
        int                             baud;           /*!< 波特率 */
        char                            parity;         /*!< 奇偶校验位 */
        int                             dataBit;        /*!< 数据位长度 */
        int                             stopBit;        /*!< 停止位长度 */
        MODBUS_MASTER_RTU_SERIAL_MODE_E mode;           /*!< 串口模式(485或232) */
} ModbusMasterSerialInfo_T;

/**
 * @brief tcp配置
 */
typedef struct
{
        char     ipv4[16]; /*!< ipv4地址 */
        uint16_t port;     /*!< 端口 */
} ModbusMasterTcpInfo_T;

/**
 * @brief rtu通道
 */
typedef struct
{
        ModbusMasterSerialInfo_T  config;      /*!< 配置文件 */
        ModbusMasterChannelInfo_T channelInfo; /*!< 通道信息 */
} ModbusMasterRtuChannel_T;

/**
 * @brief tcp通道
 */
typedef struct
{
        ModbusMasterTcpInfo_T     config;      /*!< 配置文件 */
        ModbusMasterChannelInfo_T channelInfo; /*!< 通道信息 */
} ModbusMasterTcpChannel_T;


#endif   // MODBUS_MASTER_STRUCT_H
