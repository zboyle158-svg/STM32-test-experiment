/** @file bms_hal_control.h
 *  @brief 充电、放电、唤醒、休眠和单体均衡控制接口。
 *  @details 将业务层状态转换为 BQ769x0 寄存器或 MOS GPIO 电平。
 *  @par 使用方法 由 bms_energy.c 调用；NewState 使用 BMS_STATE_ENABLE/DISABLE。
 */
#ifndef __BMS_HAL_CONTROL_H__
#define __BMS_HAL_CONTROL_H__



#include "bms_type.h"


void BMS_HalCtrlWakeup(void);
void BMS_HalCtrlSleep(void);
void BMS_HalCtrlDischarge(BMS_StateTypedef NewState);
void BMS_HalCtrlCharge(BMS_StateTypedef NewState);
void BMS_HalCtrlCellsBalance(BMS_CellIndexTypedef CellIndex, BMS_StateTypedef NewState);



#endif


