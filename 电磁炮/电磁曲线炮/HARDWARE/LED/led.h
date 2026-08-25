/**
 * @file    HARDWARE/LED/led.h
 * @brief   LED 指示灯控制模块。
 *
 * @details 本文件封装本模块的硬件初始化、数据处理和对外接口。
 *          具体引脚、通信参数和时序以当前实现及工程配置为准。
 */

#ifndef __LED_H

#define __LED_H

#include "sys.h"

#define LED0 PBout(0)// LED0：PB0 输出位

#define LED1 PBout(1)// LED1：PB1 输出位

void LED_Init(void);//初始化

#endif
