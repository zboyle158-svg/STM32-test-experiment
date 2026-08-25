/**
* @file KT6328.h
* @brief OV-Watch板级外设驱动实现。
* @details
* 本文件仅实现本模块职责；底层硬件访问通过对应HAL/BSP接口完成。
* 修改本文件时应保持公开接口、返回值约定、时序和硬件引脚配置不变。
*/

#ifndef __KT6328_H__
#define __KT6328_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

    void KT6328_GPIO_Init(void);
    void KT6328_Enable(void);
    void KT6328_Disable(void);


#ifdef __cplusplus
}
#endif
#endif

