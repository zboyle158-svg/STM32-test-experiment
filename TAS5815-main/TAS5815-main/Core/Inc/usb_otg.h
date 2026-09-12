/* USER CODE BEGIN Header */
/** @details 声明USB OTG FS句柄和初始化接口；USB插入检测由gpio.h中的PB0输入完成。 */
/**
  ******************************************************************************
  * @file    usb_otg.h
  * @brief   This file contains all the function prototypes for
  *          the usb_otg.c file
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
#ifndef __USB_OTG_H__
#define __USB_OTG_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/** @brief USB OTG FS设备控制器句柄；仅代表底层PCD。 */
extern PCD_HandleTypeDef hpcd_USB_OTG_FS;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

/** @brief 初始化USB OTG FS PCD、DM/DP引脚和USB中断。 */
void MX_USB_OTG_FS_PCD_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USB_OTG_H__ */
