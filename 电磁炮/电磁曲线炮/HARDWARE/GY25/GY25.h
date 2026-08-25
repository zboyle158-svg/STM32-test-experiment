/**
 * @file    HARDWARE/GY25/GY25.h
 * @brief   GY-25 姿态传感器串口通信模块。
 *
 * @details 本文件封装本模块的硬件初始化、数据处理和对外接口。
 *          具体引脚、通信参数和时序以当前实现及工程配置为准。
 */

#ifndef __GY25_H

#define __GY25_H

#include "stdio.h"

#include "sys.h"

#include "led.h"

#include "delay.h"

#include "key.h"

#include "sys.h"

#define USART_REC_LEN2  20  	//定义最大接收字节数 20

#define EN_USART2_RX 	1		//使能(1)/禁止(0)串口1接收

extern u8  USART_RX_BUF2[USART_REC_LEN2]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符

extern u8 USART_RX_STA2;         		//接收状态标记

//如果想串口中断接收，请不要注释以下宏定义

void uart_init2(u32 bound);

#endif

void GY25(void);

extern	float YAW;

extern	float PITCH;

extern	float ROLL;
