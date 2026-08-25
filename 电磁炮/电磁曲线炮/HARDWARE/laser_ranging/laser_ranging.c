/**
 * @file    HARDWARE/laser_ranging/laser_ranging.c
 * @brief   激光测距模块与串口数据处理。
 *
 * @details 本文件封装本模块的硬件初始化、数据处理和对外接口。
 *          具体引脚、通信参数和时序以当前实现及工程配置为准。
 */

#include "laser_ranging.h"

#include "delay.h"

#include "math.h"

#include "HMI.h"				//串口屏

#include "sys.h"

#if EN_USART3_RX   //如果使能了接收   //串口3中断服务程序

u8 USART_RX_BUF3[USART_REC_LEN3];  //接收缓冲,定义最大接收字节数 10

u8 USART_RX_STA3=0;       //接收状态标记

void uart_init3(u32 bound)

{

  //GPIO端口设置

  GPIO_InitTypeDef GPIO_InitStructure;

	USART_InitTypeDef USART_InitStructure;

	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);	//使能USART1，GPIOA时钟

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	//USART3_TX：PB10

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出

  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化 PB10

  //USART1_RX	  GPIOB.11初始化

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB.11

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入

  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化 PB11

  //USART3 的 NVIC 配置

  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//USART3 抢占优先级为 2

	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能

	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

  //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式

	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位

	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位

	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART3, &USART_InitStructure); //初始化 USART3

  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断

  USART_Cmd(USART3, ENABLE);                    //使能 USART3

}

char ddd[20];

// 测距模块通过USART3发送ASCII帧；中断只接收并标记完整帧。

void USART3_IRQHandler(void)                	//串口3中断服务程序

{

	u8 Res ;

	static u8 str=0;

#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.

	OSIntEnter();

#endif

	if(USART_GetITStatus(USART3, USART_IT_RXNE))  //接收中断

	{

		Res = USART_ReceiveData(USART3);	//读取接收到的数据

		if(Res=='D')					//以'D'开头

		{

			str=0;

			USART_RX_BUF3[str] = Res;

			USART_RX_STA3=1;

		}

		else if(USART_RX_STA3==1)

		{

			str ++;

			USART_RX_BUF3[str] = Res;

			if(Res=='m')				//以'm'结尾

			{

				USART_RX_STA3=100;

			}

		}

  }

#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.

	OSIntExit();

#endif

}

#endif

//将形如 D=12.34m 的测距帧解析为整数厘米，供自动扫描流程比较阈值。\n//返回 0 表示帧头不匹配；当前实现按固定 ASCII 格式读取，输入应来自已完成组帧的缓冲区。

int ranging(void)					//激光测距

{

	char i,a=0;

	float distance = 0;

	if((USART_RX_BUF3[0]=='D') && (USART_RX_BUF3[1]=='=') )

	{

		for(i=2;i<10;i++)

		{

			if(USART_RX_BUF3[i]=='.')  { a = i;	continue ;}

			else if(USART_RX_BUF3[i]=='m')  {if(a!=0)distance = distance * pow(10,a-i+1); break;}

			else distance = distance*10 + USART_RX_BUF3[i]-0X30;

		}

		distance *=  100;			//单位为 CM

		return	distance;

	}

	return 0;

}

void senddata(char *p,char n)//通过 USART3 发送 n 个字节，不自动追加结束符

{

	if(p == 0) return ;

	if(n > 0)

	{

		while(n --)

		{

			while((USART3->SR&0X40)==0);//循环发送,直到发送完毕

			USART3->DR =  *p++;

		}

	}

}
