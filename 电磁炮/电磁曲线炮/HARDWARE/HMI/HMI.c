/**
 * @file    HARDWARE/HMI/HMI.c
 * @brief   触摸屏人机界面与串口命令处理模块。
 *
 * @details 本文件封装本模块的硬件初始化、数据处理和对外接口。
 *          具体引脚、通信参数和时序以当前实现及工程配置为准。
 */

#include "HMI.h"

#include "sys.h"

#include "led.h"

#include "servo.h"

#include "math.h"

#include "user.h"

#include "laser_ranging.h"

#if SYSTEM_SUPPORT_OS

#include "includes.h"					//ucos 使用

#endif

//加入以下代码,支持printf函数,而不需要选择use MicroLIB

#if 1

#pragma import(__use_no_semihosting)

//标准库需要的支持函数

struct __FILE

{

	int handle;

};

FILE __stdout;

//定义_sys_exit()以避免使用半主机模式

void _sys_exit(int x)

{

	x = x;

}

//重定义fputc函数

int fputc(int ch, FILE *f)

{

	while((USART1->SR&0X40)==0){};//循环发送,直到发送完毕

    USART1->DR = (u8) ch;

	return ch;

}

#endif

#if EN_USART1_RX   //如果使能了接收

u8 USART_RX_BUF1[USART_REC_LEN1];     //接收缓冲,定义最大接收字节数 20

u16 USART_RX_STA1=0;       //接收状态标记

void uart_init1(u32 bound)

{

  //GPIO端口设置

  GPIO_InitTypeDef GPIO_InitStructure;

	USART_InitTypeDef USART_InitStructure;

	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟

	//USART1_TX   GPIOA.9

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9

  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出

  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9

  //USART1_RX	  GPIOA.10初始化

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入

  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10

  //Usart1 NVIC 配置

  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;

	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//USART1 抢占优先级为 0

	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		//子优先级3

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能

	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率

	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式

	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位

	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位

	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制

	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1

  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断

  USART_Cmd(USART1, ENABLE);                    //使能串口1

}

//USART1 每收到 1 个字节进入一次中断；中断只接收并组帧，不执行舵机、发射等耗时动作。

void USART1_IRQHandler(void)                	//串口1中断服务程序

{

	u8 Res;

	static u8 str=0;

#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.

	OSIntEnter();

#endif

	if(USART_GetITStatus(USART1, USART_IT_RXNE))  //接收中断

	{

		Res = USART_ReceiveData(USART1);	//读取接收到的数据

		if(Res=='D')			//以'D'开头

		{

			str=0;

			USART_RX_BUF1[str] = Res;

			USART_RX_STA1=1;

		}

		else if(USART_RX_STA1==1)

		{

			str ++;

			USART_RX_BUF1[str] = Res;

			if(Res=='!')		//以'!'结尾

			{

				USART_RX_STA1 = 100;

			}

		}

  }

#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.

	OSIntExit();

#endif

}

#endif

char hmibuf[20];

// 命令解释器：把中断中组好的ASCII帧转换为舵机、发射和测距参数动作。

void runHMICommand()			//触摸屏解析函数

{

	char i=0,chx=0;		//举例：D,1,100 : 控制舵机，通道1输出，PWM值100

	float	num = 0,num1 = 0;

	if(USART_RX_STA1 == 100)

	{

		USART_RX_STA1 = 0;

		chx = USART_RX_BUF1[1];

		if(chx >='0' && chx<='9')			//控制舵机指令

		{

			num = compute(USART_RX_BUF1,2,'!');

			servo(chx-0x30,num);		//调用舵机控制函数

		}

		else if(chx>='A'&&chx<='Z')		//控制射击

		{

			if(chx == 'A')		shoot();   //发射

			else if(chx == 'P')

			{

				num = compute(USART_RX_BUF1,2,'!') * 10;

				TIM_SetCompare2(TIM5,(int)num);

			}

			else if(chx == 'B')			//输入距离,发射至指定距离  一、(2)、(3)

			{

				tance_1(compute(USART_RX_BUF1,2,'!'));

			}

			else if(chx == 'C')			//输入距离及角度,发射至靶心		二、(1)

			{

				num = compute(USART_RX_BUF1,2,'R');

				for(i=2;i<20;i++)

				{

					if(USART_RX_BUF1[i]=='R')

					{

						num1 = compute(USART_RX_BUF1,++i,'!');

						break ;

					}

				}

				tance_2(num,num1);

			}

			else if(chx == 'E')				// 二、(2)

			{

				senddata("iFACM:2",7);	//自动持续测量

				tance_3(USART_RX_BUF1[2]-0x30);		//自动模式

			}

			else if(chx == 'F')			//激光距离比较值

			{

				flage	= compute(USART_RX_BUF1,2,'!');

			}

			else if(chx == 'G')			//激光扫描范围

			{

				fan	= compute(USART_RX_BUF1,2,'!');

			}

			else if(chx == 'H')			//激光寻靶距离补偿

			{

				tit	= compute((unsigned char*)USART_RX_BUF1,2,'!');

			}	//数组名为变量,若有指针指向它时,必须在强制转化类型的后面加 ' * '

		}

	}

}

//解析 ASCII 十进制数，支持可选负号和小数点；n 是起始下标，m 是结束字符。\n//例如 D3-12.5! 传入 n=2、m=! 时，返回 -12.5；调用方需要保证帧格式合法且长度不超过缓冲区。

float compute(unsigned char *p,char n, char m)   //计算任意一个带符号的十进制数组,起始位,结束位

{

	char i=0,a=0,b=0;

	float num=0;

	p += n;

	if(*p =='-') {p++; b = 1;}

	for(i=0;i<10;i++)

	{

		if(*p == '.')  {a = i; p++; continue;}

		else if(*p == m)  {if(a!=0) num = num * pow(10.0,a-i+1); break;}

		else num = num * 10 + *p - 0X30;

		p++;

	}

	if(b==1) num *= -1;

	return num;

}

char endCode[4]={0xff,0xff,0xff,0};

void sendData(char *p,uchar n)//通过 USART1 发送 n 个字节；函数不会自动追加结束符

{

	if(p == 0) return ;

	while(n --)

	{

		while((USART1->SR&0X40)==0){};//循环发送,直到发送完毕

		USART1->DR =  *p++;

	}

}

void sendStringHMI(char *p)	//发送串口屏命令

{

   sendData(p,strlen(p));

	 sendData(endCode,3);

}

/*

	iACM   自动持续测量

	iHALT	 停止持续测量

*/
