/**
* @file delay.h
* @brief IAP系统基础功能实现。
* @details
* 本文件仅实现本模块职责；底层硬件访问通过对应HAL/BSP接口完成。
* 修改本文件时应保持公开接口、返回值约定、时序和硬件引脚配置不变。
*/

#ifndef _DELAY_H
#define _DELAY_H
#include "sys.h"

void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);
#endif

