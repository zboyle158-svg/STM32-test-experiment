/* USER CODE BEGIN Header */
/** @details 对外提供TAS5815初始化、音量、静音、增益、调制、FSW和睡眠控制接口。调用者只传入逻辑索引，不直接操作芯片寄存器。 */
/**
  ******************************************************************************
  * @file    tas5815.h
  * @brief   Header file for TAS5815 driver functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Your Company/Name.
  * All rights reserved.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TAS5815_H
#define __TAS5815_H

/* Includes ------------------------------------------------------------------*/
#include "main.h" /* HAL library and necessary definitions */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* TAS5815 I2C Address (7-bit: 0x54, 8-bit write: 0xA8) */
/**
 * @brief TAS5815的7位I2C从地址。
 * @note HAL旧版I2C接口通常要求传入左移一位后的地址，驱动内部使用0x54 << 1。
 */
#define TAS5815_I2C_ADDR_7BIT   0x54

/* Exported functions --------------------------------------------------------*/
/** @brief 使用HAL时间基准阻塞指定毫秒数。 @param ms 延时时间，单位ms。 */
void delay_ms(uint32_t ms);
/** @brief 按数据手册顺序初始化TAS5815并进入播放状态。 */
void tas5815_Initialize(void);
/** @brief 将应用音量等级转换为TAS5815数字音量寄存器值。 @param count 音量等级，0表示静音。 */
void set_tas5815_volume(uint32_t count);
/** @brief 设置TAS5815为播放状态下的静音状态。 */
void tas5815_mute(void);
/** @brief 取消TAS5815静音并恢复播放状态。 */
void tas5815_unmute(void);
/** @brief 设置模拟增益。 @param gain_index 增益表索引，范围0~7。 */
void tas5815_set_analog_gain(uint8_t gain_index);
/** @brief 设置调制模式。 @param modulation 0=BD，1=1SPW，3=Hybrid。 */
void tas5815_set_modulation(uint8_t modulation);
/** @brief 设置功率级开关频率。 @param fsw_sel 当前支持0=768kHz、1=384kHz。 */
void tas5815_set_fsw(uint8_t fsw_sel);
/** @brief 让TAS5815进入低功耗或非播放状态。 */
void tas5815_sleep(void);
/** @brief 让TAS5815从休眠状态按时序恢复到播放状态。 */
void tas5815_wakeup(void);

#endif /* __TAS5815_H */

/************************ (C) COPYRIGHT Your Company *****END OF FILE****/
