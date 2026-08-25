/**
 * @file    HARDWARE/servo/servo.c
 * @brief   舵机 PWM 控制模块。
 *
 * @details 本文件封装本模块的硬件初始化、数据处理和对外接口。
 *          具体引脚、通信参数和时序以当前实现及工程配置为准。
 */

#include "servo.h"

#include "led.h"

#include "HMI.h"

#include "math.h"

//TIM3 PWM部分初始化

//PWM输出初始化

//arr：自动重装值

//psc：时钟预分频数

// TIM4 CH3/CH4通过PB8/PB9输出两路舵机控制脉冲，周期约20ms。

void TIM4_PWM_Init(u16 arr,u16 psc)

{

	GPIO_InitTypeDef GPIO_InitStructure;

	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 | RCC_APB2Periph_GPIOB, ENABLE);	//使能定时器3时钟    1

	//GPIOB 已由工程初始化流程配置；PB8/PB9 作为 TIM4 复用输出。

  //设置该引脚为复用输出功能,输出TIM4 CH3的PWM脉冲波形

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;   //TIM_CH3、CH4

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出    4

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);    //初始化GPIO     3

   //配置 TIM4 时间基准

	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值

	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值

	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim

	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式

	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	//配置 TIM4 输出比较通道为 PWM2 模式

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2

 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能

	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低

	TIM_OC3Init(TIM4,&TIM_OCInitStructure);  //初始化 TIM4 CH3（PB8）

	TIM_OC4Init(TIM4,&TIM_OCInitStructure);  //初始化 TIM4 CH4（PB9）

	TIM_OC3PreloadConfig(TIM4,TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器    8

	TIM_OC4PreloadConfig(TIM4,TIM_OCPreload_Enable);

	TIM_SetCompare3(TIM4,1510);		//上部舵机

	TIM_SetCompare4(TIM4,1500);		//下部舵机

	TIM_Cmd(TIM4, ENABLE);  //启动 TIM4 计数器

}

float angle=0;

//x=3 控制俯仰舵机（TIM4 CH3/PB8）；x=4 控制水平舵机（TIM4 CH4/PB9）；x=2 两路回中。\n//b 的单位是项目标定使用的角度值，pwm 是装入 CCR 的计数值。

void servo(char x, float b)	//通道、动态移动、固定移动

{

	int pwm =0;

	if(x==3)

	{

		if(b < 0)

			pwm = b* 10.722 * -1 +1510;	//下负

		else

			pwm = 1510-b * 10.888;  			//上正

		TIM_SetCompare3(TIM4,pwm);

	}

	else if(x==4)

	{

		angle = b;

		if(angle < 0)

			pwm = 1500-angle * 10.444 * -1;	//左正

		else

			pwm = angle * 11.111+1500;  		//右负

		TIM_SetCompare4(TIM4,pwm);

	}

	else if(x==2)

	{

		angle = 0;

		TIM_SetCompare3(TIM4,1510);    //垂直	B8

		TIM_SetCompare4(TIM4,1500); 	 //水平 B9

	}

}
