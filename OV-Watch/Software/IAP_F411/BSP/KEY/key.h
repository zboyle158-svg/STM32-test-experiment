/**
* @file key.h
* @brief OV-Watch板级外设驱动实现。
* @details
* 本文件仅实现本模块职责；底层硬件访问通过对应HAL/BSP接口完成。
* 修改本文件时应保持公开接口、返回值约定、时序和硬件引脚配置不变。
*/

#ifndef __KEY_H__
#define __KEY_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

    //KEY1
#define KEY1_PORT	GPIOA
#define KEY1_PIN	GPIO_PIN_5
#define KEY1 HAL_GPIO_ReadPin(KEY1_PORT,KEY1_PIN)
    //KEY_Wake
#define KEY2_PORT	GPIOA
#define KEY2_PIN	GPIO_PIN_4
#define KEY2 HAL_GPIO_ReadPin(KEY2_PORT,KEY2_PIN)

    void Key_Port_Init(void);
    void Key_Interrupt_Callback(void);
    uint8_t KeyScan(uint8_t mode);




#ifdef __cplusplus
}
#endif
#endif

