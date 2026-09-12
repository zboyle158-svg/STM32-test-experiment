/** @file bms_hal_monitor.h
 *  @brief BQ769x0 采样数据到 BMS 监测结构体的转换接口。
 *  @details 提供电压、电流、温度和负载检测函数，供 bms_monitor 任务调用。
 */
#ifndef __BMS_HAL_SAMPLE_H__
#define __BMS_HAL_SAMPLE_H__


#include <stdbool.h>

#include "bms_type.h"
#include "bms_config.h"


void Bms_HalMonitorCellVoltage(void);
void Bms_HalMonitorBatteryVoltage(void);
void Bms_HalMonitorBatteryCurrent(void);
void Bms_HalMonitorCellTemperature(void);
bool Bms_HalMonitorLoadDetect(void);


#endif

