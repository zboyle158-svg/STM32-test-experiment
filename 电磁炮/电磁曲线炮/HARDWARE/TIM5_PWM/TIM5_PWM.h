/**
 * @file    HARDWARE/TIM5_PWM/TIM5_PWM.h
 * @brief   发射控制 PWM 输出模块。
 *
 * @details 本文件封装本模块的硬件初始化、数据处理和对外接口。
 *          具体引脚、通信参数和时序以当前实现及工程配置为准。
 */

#ifndef __TIMER_H

#define __TIMER_H

#include "sys.h"

void TIM5_PWM_Init(u16 arr,u16 psc);

#endif
