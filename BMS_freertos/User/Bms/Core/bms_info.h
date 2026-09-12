/** @file bms_info.h
 *  @brief BMS 状态信息输出任务接口。
 *  @details 周期打印采样、保护、SOC 和能量状态，主要用于串口调试与现场诊断。
 *  @par 使用方法 初始化阶段调用 BMS_InfoInit()；输出接口由 bms_debug.h 配置。
 */
#ifndef __BMS_INFO_H__
#define __BMS_INFO_H__

void BMS_InfoInit(void);


#endif


