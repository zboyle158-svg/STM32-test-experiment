/**
* @file WDOG.h
* @brief OV-Watch板级外设驱动实现。
* @details
* 本文件仅实现本模块职责；底层硬件访问通过对应HAL/BSP接口完成。
* 修改本文件时应保持公开接口、返回值约定、时序和硬件引脚配置不变。
*/

#ifndef __WDOG_H__
#define __WDOG_H__

#ifdef __cplusplus
extern "C" {
#endif

    void WDOG_Port_Init(void);
    void WDOG_Enable(void);
    void WDOG_Disnable(void);
    void WDOG_Feed(void);


#ifdef __cplusplus
}
#endif
#endif

