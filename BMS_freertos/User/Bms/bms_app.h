#ifndef __BMS_APP_H__
#define __BMS_APP_H__

/**
 * @file bms_app.h
 * @brief BMS 应用层总初始化接口。
 * @details 由 RTOS 启动任务调用，负责串联驱动、硬件抽象和业务任务初始化。
 */

/**
 * @brief 初始化 BQ769x0、软件 I2C 及全部 BMS 业务任务。
 * @note 只能在 RTOS 内核启动后调用一次，重复调用会重复创建任务。
 */
void BMS_SysInitialize(void);

#endif

