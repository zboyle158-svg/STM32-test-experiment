/**
 * @file    HARDWARE/ADC/adc.h
 * @brief   ADC 模拟量采集模块。
 *
 * @details 本文件封装本模块的硬件初始化、数据处理和对外接口。
 *          具体引脚、通信参数和时序以当前实现及工程配置为准。
 */

#ifndef __ADC_H

#define __ADC_H

#include "sys.h"

void Adc_Init(void);

u16 Get_Adc(u8 ch);

float Get_Adc_Average(u8 ch, u8 times);  //第几个ADC，平均计算几次

#endif
