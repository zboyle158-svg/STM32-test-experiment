/**
* @file AHT21.h
* @brief OV-Watch板级外设驱动实现。
* @details
* 本文件仅实现本模块职责；底层硬件访问通过对应HAL/BSP接口完成。
* 修改本文件时应保持公开接口、返回值约定、时序和硬件引脚配置不变。
*/

#ifndef __AHT21_H
#define __AHT21_H

#include "stm32f4xx_hal.h"
#include "iic_hal.h"
#include "delay.h"

uint8_t AHT_Read_Status(void);
void AHT_Reset(void);
uint8_t AHT_Init(void);
uint8_t AHT_Read(float *humi, float *temp);

#endif
