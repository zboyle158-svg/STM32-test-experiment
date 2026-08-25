/**
* @file user_SensUpdateTask.h
* @brief OV-Watch FreeRTOS任务与任务间通信实现。
* @details
* 本文件仅实现本模块职责；底层硬件访问通过对应HAL/BSP接口完成。
* 修改本文件时应保持公开接口、返回值约定、时序和硬件引脚配置不变。
*/

#ifndef __USER_SENSUPDATETASK_H__
#define __USER_SENSUPDATETASK_H__

#ifdef __cplusplus
extern "C" {
#endif

    extern uint32_t user_HR_timecount;

    void SensorDataUpdateTask(void *argument);
    void HRDataUpdateTask(void *argument);
    void MPUCheckTask(void *argument);

#ifdef __cplusplus
}
#endif

#endif

