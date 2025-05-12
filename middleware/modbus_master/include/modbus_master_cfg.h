/**
* @file
 * @brief       modbus master 宏参数与枚举
 * @author      ylb
 * @date        2025-04-25
 * @version     v1.0
 * @par History:
 *	version v1.0: 基础程序首次创建\n
 */

#ifndef MODBUS_MASTER_CFG_H
#define MODBUS_MASTER_CFG_H

/** MODBUS MASTER 组建版本号 */
#define MODBUS_MASTER_VERSION "1.0.0"

/** MODBUS MASTER 运行信息输出宏接口 */
#define MODBUS_MASTER_INFO_PRINT(fmt, ...) printf(fmt, ##__VA_ARGS__)

/** MODBUS MASTER 启用断言 */
#define MODBUS_MASTER_ASSERT 0

/** MODBUS MASTER 启用运行信息输出 */
#define MODBUS_MASTER_DEBUG_PRINT 1

/** MODBUS MASTER 消息帧数据域长度 */
#define MODBUS_MASTER_PDU_MAX 253
/** MODBUS MASTER 单信道最大从设备数量 */
#define MODBUS_MASTER_MAX_SLAVE_NUM 255

/** MODBUS MASTER 寄存器类型 */
typedef enum
{
        MODBUS_MASTER_HOLDING_REG,        /*!< 保持寄存器 */
        MODBUS_MASTER_INPUT_REG,          /*!< 输入寄存器 */
        MODBUS_MASTER_COIL_STATUS_REG,    /*!< 线圈 */
        MODBUS_MASTER_INPUT_STATUS_REG,   /*!< 离散输入 */
} MODBUS_MASTER_REG_TYPE_E;

/** MODBUS MASTER 寄存器翻转步长 */
typedef enum
{
        MODBUS_MASTER_FILP_0 = 0,
        MODBUS_MASTER_FILP_2 = 2,
        MODBUS_MASTER_FILP_4 = 4,
        MODBUS_MASTER_FILP_8 = 8,
} MODBUS_MASTER_REG_FILP_STEP_E;

typedef enum
{
        MODBUS_MASTER_RTU_RS232 = 0,
        MODBUS_MASTER_RTU_RS485 = 1,
}MODBUS_MASTER_RTU_SERIAL_MODE_E;

#endif   // MODBUS_MASTER_CFG_H
