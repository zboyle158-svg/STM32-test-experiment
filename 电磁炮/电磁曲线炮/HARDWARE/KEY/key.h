/**
 * @file    HARDWARE/KEY/key.h
 * @brief   按键输入模块。
 *
 * @details 本文件封装本模块的硬件初始化、数据处理和对外接口。
 *          具体引脚、通信参数和时序以当前实现及工程配置为准。
 */

#ifndef __KEY_H

#define __KEY_H

#include "sys.h"

//#define KEY0 PEin(4)   	//PE4

//#define KEY1 PEin(3)	//PE3

//#define KEY2 PEin(2)	//PE2

//#define WK_UP PAin(0)	//PA0  WK_UP

#define KEY0  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)//读取按键0

#define KEY1  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9)//读取按键1

#define KEY2  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10)//读取按键2

#define KEY3  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_11)//读取按键3

#define KEY0_PRES 1	//KEY0按下

#define KEY1_PRES	2	//KEY1按下

#define KEY2_PRES	3	//KEY2按下

#define KEY3_PRES 4	//KEY3按下

void KEY_Init(void);//IO初始化

u8 KEY_Scan(u8);  	//按键扫描函数

#endif
