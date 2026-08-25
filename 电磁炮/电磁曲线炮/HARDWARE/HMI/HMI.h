/**
 * @file    HARDWARE/HMI/HMI.h
 * @brief   触摸屏人机界面与串口命令处理模块。
 *
 * @details 本文件封装本模块的硬件初始化、数据处理和对外接口。
 *          具体引脚、通信参数和时序以当前实现及工程配置为准。
 */

#ifndef _HMI_H__

#define _HMI_H__

#include <stdarg.h>		  //  标准头文件

#include <string.h>

#include "delay.h"

#include <stdio.h>

#include "sys.h"

#define USART_REC_LEN1  20  	//定义最大接收字节数 20

#define EN_USART1_RX 	1		//使能(1)/禁止(0)串口1接收

//#define _CAMETA_MODE_

extern u8  USART_RX_BUF1[USART_REC_LEN1]; //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符

extern u16 USART_RX_STA1;         		//接收状态标记

//如果想串口中断接收，请不要注释以下宏定义

void uart_init1(u32 bound);

typedef unsigned int  uint;			//定义无符号整型变量为uint

typedef unsigned char  uchar;		//定义无符号字符型变量为uchar

void sendData(char *p,uchar n);

void sendStringHMI(char *p);

void runHMICommand(void);

float compute(unsigned char *p,char n, char m);

#endif
