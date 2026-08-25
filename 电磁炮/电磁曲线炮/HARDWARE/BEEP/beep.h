/**
 * @file    HARDWARE/BEEP/beep.h
 * @brief   蜂鸣器控制模块。
 *
 * @details 本文件封装本模块的硬件初始化、数据处理和对外接口。
 *          具体引脚、通信参数和时序以当前实现及工程配置为准。
 */

#ifndef __BEEP_H

#define __BEEP_H

#include "sys.h"

#define BEEP PAout(12)	// BEEP,蜂鸣器接口

void BEEP_Init(void);	//初始化

#endif
