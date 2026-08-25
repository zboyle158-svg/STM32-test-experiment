/**
 * @file    HARDWARE/BEEP/beep.c
 * @brief   蜂鸣器控制模块。
 *
 * @details 本文件封装本模块的硬件初始化、数据处理和对外接口。
 *          具体引脚、通信参数和时序以当前实现及工程配置为准。
 */

#include "beep.h"

void BEEP_Init(void)

{

//	GPIO_InitTypeDef   GPIO_InitStructure;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //使GPIO能时钟

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;         //端口配置

//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //输出模式

//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;    //速度50MHZ

//	GPIO_Init(GPIOA,&GPIO_InitStructure);             //初始化GPIOA。12

//	GPIO_SetBits(GPIOA,GPIO_Pin_12);

}
