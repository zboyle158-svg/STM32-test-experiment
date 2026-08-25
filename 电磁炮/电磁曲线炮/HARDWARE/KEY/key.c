/**
 * @file    HARDWARE/KEY/key.c
 * @brief   按键输入模块。
 *
 * @details 本文件封装本模块的硬件初始化、数据处理和对外接口。
 *          具体引脚、通信参数和时序以当前实现及工程配置为准。
 */

#include "stm32f10x.h"

#include "key.h"

#include "sys.h"

#include "delay.h"

//按键初始化函数

void KEY_Init(void) //IO初始化

{

 	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);//使能 GPIOD 时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;//KEY0～KEY3 对应 PD8～PD11

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//PA0设置成输入，默认下拉,,IPU为上拉

 	GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化 PD8～PD11

}

//按键处理函数

//返回按键值

//mode=0：一次按下只返回一次；mode=1：允许按住时连续返回。

//0，没有任何按键按下

//多个键同时按下时按 KEY0、KEY1、KEY2、KEY3 的顺序判断，KEY0 优先级最高。

// 轮询按键并做约1ms消抖；当前main.c没有调用它，主要入口是USART1触摸屏。

u8 KEY_Scan(u8 mode)

{

	static u8 key_up=1;//按键按松开标志

	if(mode)key_up=1;  //支持连按

	if(KEY0==0||KEY1==0||KEY2==0||KEY3==0)

	{

		delay_ms(1);//去抖动

		if(key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==0))

		{

			key_up=0;

			if(KEY0==0)				return	KEY0_PRES;

			else if(KEY1==0)	return	KEY1_PRES;

			else if(KEY2==0)	return	KEY2_PRES;

			else if(KEY3==0)	return	KEY3_PRES;

		}

	}

	else	key_up=1;

 	return 0;// 无按键按下

}
