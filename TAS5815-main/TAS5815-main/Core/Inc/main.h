/* USER CODE BEGIN Header */
/** @details 本项目公共应用头文件：包含HAL、GPIO宏、错误处理声明以及main.c对外提供的音量/电压控制接口。 */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
/* 音量控制函数（供菜单系统同步硬件计数器使用） */
/**
 * @brief 获取当前音量逻辑值。
 * @return 音量等级，范围为0~99；该值不是TAS5815的原始寄存器值。
 */
int32_t GetVolume(void);

/**
 * @brief 设置当前音量，并同步TIM2编码器计数器。
 * @param vol 目标音量等级，函数内部会限制到0~99。
 * @note TIM2每两个硬件计数对应一个音量等级，因此计数器会被写为vol << 1。
 */
void SetVolume(int32_t vol);

/**
 * @brief 设置TAS5815调制模式并更新应用层缓存。
 * @param modulation TAS5815调制模式编码，当前工程使用0、1和3。
 */
void SetModulation(uint8_t modulation);

/**
 * @brief 设置TAS5815功率级开关频率并更新应用层配置。
 * @param fsw FSW选择编码，当前菜单使用0和1。
 */
void SetFSW(uint8_t fsw);

/**
 * @brief 设置CH224K PD诱骗器的输出电压档位。
 * @param voltage 电压档位编码：0为5V，1为12V，2为高压档。
 * @note 实际高压值必须以硬件CH224K配置和万用表测量结果为准。
 */
void SetVoltage(uint8_t voltage);

/**
 * @brief 按当前电压缓存重新驱动PD控制GPIO。
 * @note 用于STOP模式唤醒后恢复进入低功耗前的工作电压档位。
 */
void RestoreVoltage(void);
/* 关机/唤醒函数 */
/**
 * @brief 保存必要设置并让系统进入STOP低功耗模式。
 * @note PB9编码器按键和PB0 USB检测输入被配置为唤醒源；唤醒后函数会恢复时钟、显示、蓝牙、功放和音量。
 */
void System_PowerOff(void);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/

/* USER CODE BEGIN Private defines */
/* 编码器按键 - PB9 */
/** @brief 编码器按键输入，引脚为PB9；正常工作时由菜单模块轮询。 */
#define ENC_KEY_Pin GPIO_PIN_9
#define ENC_KEY_GPIO_Port GPIOB

/* 蓝牙控制引脚 - QCC3034 */
#define BT_PAIR_Pin GPIO_PIN_12      /* PB12 - 暂停/蓝牙配对 */
#define BT_PAIR_GPIO_Port GPIOB
#define BT_VOL_DOWN_Pin GPIO_PIN_13  /* PB13 - VOL-/上一曲 */
#define BT_VOL_DOWN_GPIO_Port GPIOB
#define BT_VOL_UP_Pin GPIO_PIN_14    /* PB14 - VOL+/下一曲 */
#define BT_VOL_UP_GPIO_Port GPIOB
#define BT_USB_DET_Pin GPIO_PIN_0    /* PB0 - 蓝牙USB口检测 */
#define BT_USB_DET_GPIO_Port GPIOB
#define BT_PLAY_DET_Pin GPIO_PIN_6   /* PA6 - 播放检测 */
#define BT_PLAY_DET_GPIO_Port GPIOA
#define BT_LED0_Pin GPIO_PIN_15      /* PB15 - LED0 */
#define BT_LED0_GPIO_Port GPIOB
#define BT_LED1_Pin GPIO_PIN_4       /* PA4 - LED1 */
#define BT_LED1_GPIO_Port GPIOA
#define BT_EN_Pin GPIO_PIN_10        /* PA10 - 蓝牙模块启动，高电平有效 */
#define BT_EN_GPIO_Port GPIOA

/* PD诱骗控制引脚 - CH224K */
#define PD_5V_Pin GPIO_PIN_9         /* PA9 - 5V控制，高电平有效 */
#define PD_5V_GPIO_Port GPIOA
#define PD_20V_Pin GPIO_PIN_10       /* PB10 - 20V控制，高电平有效 */
#define PD_20V_GPIO_Port GPIOB
#define PD_VOLT_DET_Pin GPIO_PIN_1   /* PB1 - 电压检测ADC (分压比: 51k/10k) */
#define PD_VOLT_DET_GPIO_Port GPIOB

/* 12V是默认状态：PD_5V=0, PD_20V=0 */
/* 电压检测计算: 实际电压 = ADC值 * 3.3V / 4095 * 6.1 */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
