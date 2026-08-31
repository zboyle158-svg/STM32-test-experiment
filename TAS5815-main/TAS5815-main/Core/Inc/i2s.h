/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    i2s.h
  * @brief   This file contains all the function prototypes for
  *          the i2s.c file
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
#ifndef __I2S_H__
#define __I2S_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/** @brief I2S1句柄，底层使用SPI1外设承载数字音频时钟和数据。 */
extern I2S_HandleTypeDef hi2s1;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/** @brief 初始化I2S1主机发送模式和48kHz音频接口参数。 */
void MX_I2S1_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2S_H__ */
