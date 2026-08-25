/**
 * @file    HARDWARE/TIM5_PWM/TIM5_PWM.c
 * @brief   发射控制 PWM 输出模块。
 *
 * @details 本文件封装本模块的硬件初始化、数据处理和对外接口。
 *          具体引脚、通信参数和时序以当前实现及工程配置为准。
 */

#include "TIM5_PWM.h"

#include "led.h"

#include "usart.h"

//TIM1 CH1 PWM输出设置

//PWM输出初始化

//arr：自动重装值

//psc：时钟预分频数

// TIM5 CH2通过PA1输出发射控制PWM；初始化比较值为0以保持上电关闭。

void TIM5_PWM_Init(u16 arr,u16 psc)

{

	GPIO_InitTypeDef GPIO_InitStructure;

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //使能 TIM5 时钟

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);  //使能 GPIOA 和 AFIO 时钟

  //PA1 配置为复用推挽输出，对应 TIM5 CH2。

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //PA1：TIM5_CH2

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用功能输出

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化GPIO

	TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装载周期值

	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置预分频值 不分频

	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //CH2 使用 PWM2 模式

	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能

	TIM_OCInitStructure.TIM_Pulse = 0; //设置待装入捕获比较寄存器的脉冲值

	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //CH2 低电平有效

	TIM_OC2Init(TIM5, &TIM_OCInitStructure);  //初始化 TIM5 CH2

	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);  //CH2 预装载使能

	TIM_ARRPreloadConfig(TIM5, ENABLE); //使能TIMx在ARR上的预装载寄存器

	TIM_CtrlPWMOutputs(TIM5,ENABLE);	//MOE 主输出使能,高级定时器必须开启这个

	TIM_Cmd(TIM5, ENABLE);  //使能TIMx

	TIM_SetCompare2(TIM5,0);//初始值为0  CCR

}
