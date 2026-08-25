/**
* @file HrAlgorythm.h
* @brief OV-Watch板级外设驱动实现。
* @details
* 本文件仅实现本模块职责；底层硬件访问通过对应HAL/BSP接口完成。
* 修改本文件时应保持公开接口、返回值约定、时序和硬件引脚配置不变。
*/

#ifndef __HRALGORYTHM_H__
#define __HRALGORYTHM_H__

#include "main.h"
#include "user_Queue.h"

void HR_AlgoInit(void);
uint16_t HR_Calculate(uint16_t present_dat,uint32_t present_time);


#endif