/**
 * @file    HARDWARE/servo/servo.h
 * @brief   舵机 PWM 控制模块。
 *
 * @details 本文件封装本模块的硬件初始化、数据处理和对外接口。
 *          具体引脚、通信参数和时序以当前实现及工程配置为准。
 */

#ifndef __SERVO_H

#define __SERVO_H

#include "sys.h"

void TIM4_PWM_Init(u16 arr,u16 psc);

void servo(char x, float b);	//通道、固定移动

extern float angle;

#endif
