/**
 * @file    HARDWARE/GY25/GY25.c
 * @brief   GY-25 姿态传感器串口通信模块。
 *
 * @details 本文件封装本模块的硬件初始化、数据处理和对外接口。
 *          具体引脚、通信参数和时序以当前实现及工程配置为准。
 */

#include "GY25.h"

#include "sys.h"

#include "led.h"

#include "stdio.h"

#if EN_USART2_RX   //如果使能了接收

u8 USART_RX_BUF2[USART_REC_LEN2];     //接收缓冲,定义最大接收字节数 20

u8 USART_RX_STA2=0;       //接收状态标记

void uart_init2(u32 bound)

{

  //GPIO端口设置

  GPIO_InitTypeDef GPIO_InitStructure;

	USART_InitTypeDef USART_InitStructure;

	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2|RCC_APB2Periph_GPIOD, ENABLE);	//使能USART1，GPIOA时钟

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);		//开启复用

	GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE); 		//Timer3完全重映射   5

	//USART1_TX   GPIOA.9

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出

  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOA.9

  //USART1_RX	  GPIOA.10初始化

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入

  GPIO_Init(GPIOD, &GPIO_InitStructure);//初始化GPIOA.10

  //Usart1 NVIC 配置

  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=4 ;//抢占优先级3

	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;		//子优先级3

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能

	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式

	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位

	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位

	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART2, &USART_InitStructure); //初始化串口1

  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启串口接受中断

  USART_Cmd(USART2, ENABLE);                    //使能串口1

}

void USART2_IRQHandler(void)                	//串口2中断服务程序

{

	u8 Res;

	static u8 str=0;

#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.

	OSIntEnter();

#endif

	if(USART_GetITStatus(USART2, USART_IT_RXNE))  //接收中断

	{

		Res =USART_ReceiveData(USART2);	//读取接收到的数据

		if(Res==0xAA)

		{

			str=0;

			USART_RX_BUF2[str] = Res;

			USART_RX_STA2=1;

		}

		else if(USART_RX_STA2==1)

		{

			str ++;

			if(Res==0x55)

			{

				USART_RX_BUF2[str]='\0';

				USART_RX_STA2=100;

				return ;

			}

			USART_RX_BUF2[str] = Res;

		}

  }

#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.

	OSIntExit();

#endif

}

#endif

float YAW=0;

float PITCH=0;

float ROLL=0;

void GY25(void)

{

	short int YPR[3];

	if(USART_RX_STA2==100)

	{

		USART_RX_STA2 = 0;

		if(USART_RX_BUF2[0]==0XAA&&USART_RX_BUF2[7]==0X55)		//帧头判断

		{

			YPR[0]=(USART_RX_BUF2[1]<<8 | USART_RX_BUF2[2]);	//航向角	(YAW  ) ±180°

			YPR[1]=(USART_RX_BUF2[3]<<8 | USART_RX_BUF2[4]);	//俯仰角	(PITCH) ±180°(±90°+±90°)

			YPR[2]=(USART_RX_BUF2[5]<<8 | USART_RX_BUF2[6]);	//横滚角	(ROLL ) ±180°

			YAW = YPR[0];		YAW /=100;

			PITCH=YPR[0];	PITCH /=100;

			ROLL =YPR[0];	 ROLL /=100;

		}

	}

}
