//
// Created by three on 2025/5/8.
//

#ifndef BIT_FIELD_VARIABLE_STRUCT_H
#define BIT_FIELD_VARIABLE_STRUCT_H

#include <stdint.h>

typedef struct
{
        uint8_t bit0 : 1;  // 第0位
        uint8_t bit1 : 1;  // 第1位
        uint8_t bit2 : 1;  // 第2位
        uint8_t bit3 : 1;  // 第3位
        uint8_t bit4 : 1;  // 第4位
        uint8_t bit5 : 1;  // 第5位
        uint8_t bit6 : 1;  // 第6位
        uint8_t bit7 : 1;  // 第7位
}ByteBits_T;

typedef union
{
        uint8_t value;
        ByteBits_T bits;
}ByteBits_U;

typedef struct
{
        uint16_t bit0 : 1;  // 第0位
        uint16_t bit1 : 1;  // 第1位
        uint16_t bit2 : 1;  // 第2位
        uint16_t bit3 : 1;  // 第3位
        uint16_t bit4 : 1;  // 第4位
        uint16_t bit5 : 1;  // 第5位
        uint16_t bit6 : 1;  // 第6位
        uint16_t bit7 : 1;  // 第7位
        uint16_t bit8 : 1;  // 第8位
        uint16_t bit9 : 1;  // 第9位
        uint16_t bit10 : 1;  // 第10位
        uint16_t bit11 : 1;  // 第11位
        uint16_t bit12 : 1;  // 第12位
        uint16_t bit13 : 1;  // 第13位
        uint16_t bit14 : 1;  // 第14位
        uint16_t bit15 : 1;  // 第15位
}HalfWordBits_T;

typedef union
{
        uint16_t value;
        HalfWordBits_T bits;
}HalfWordBits_U;

typedef struct
{
        uint32_t bit0 : 1;  // 第0位
        uint32_t bit1 : 1;  // 第1位
        uint32_t bit2 : 1;  // 第2位
        uint32_t bit3 : 1;  // 第3位
        uint32_t bit4 : 1;  // 第4位
        uint32_t bit5 : 1;  // 第5位
        uint32_t bit6 : 1;  // 第6位
        uint32_t bit7 : 1;  // 第7位
        uint32_t bit8 : 1;  // 第8位
        uint32_t bit9 : 1;  // 第9位
        uint32_t bit10 : 1;  // 第10位
        uint32_t bit11 : 1;  // 第11位
        uint32_t bit12 : 1;  // 第12位
        uint32_t bit13 : 1;  // 第13位
        uint32_t bit14 : 1;  // 第14位
        uint32_t bit15 : 1;  // 第15位
        uint32_t bit16 : 1;  // 第16位
        uint32_t bit17 : 1;  // 第17位
        uint32_t bit18 : 1;  // 第18位
        uint32_t bit19 : 1;  // 第19位
        uint32_t bit20 : 1;  // 第20位
        uint32_t bit21 : 1;  // 第21位
        uint32_t bit22 : 1;  // 第22位
        uint32_t bit23 : 1;  // 第23位
        uint32_t bit24 : 1;  // 第24位
        uint32_t bit25 : 1;  // 第25位
        uint32_t bit26 : 1;  // 第26位
        uint32_t bit27 : 1;  // 第27位
        uint32_t bit28 : 1;  // 第28位
        uint32_t bit29 : 1;  // 第29位
        uint32_t bit30 : 1;  // 第30位
        uint32_t bit31 : 1;  // 第31位
}WordBits_T;

typedef union
{
        uint32_t value;
        WordBits_T bits;
}WordBits_U;

#endif //BIT_FIELD_VARIABLE_STRUCT_H
