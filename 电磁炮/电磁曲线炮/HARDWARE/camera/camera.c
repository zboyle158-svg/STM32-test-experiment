/**
 * @file    HARDWARE/camera/camera.c
 * @brief   摄像头串口通信预留模块。
 *
 * @details 本文件封装本模块的硬件初始化、数据处理和对外接口。
 *          具体引脚、通信参数和时序以当前实现及工程配置为准。
 */

#include "sys.h"

//#include "camera.h"

//#include "led.h"

//#include "servo.h"

//#if EN_USART3_RX   //如果使能了接收   //串口3中断服务程序

//u8 USART_RX_BUF3[USART_REC_LEN3];  //接收缓冲,定义最大接收字节数 10

//u8 USART_RX_STA3=0;       //接收状态标记

//void uart_init3(u32 bound)

//{

//  //GPIO端口设置

//  GPIO_InitTypeDef GPIO_InitStructure;

//	USART_InitTypeDef USART_InitStructure;

//	NVIC_InitTypeDef NVIC_InitStructure;

//

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);	//使能USART1，GPIOA时钟

//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

//	//USART1_TX   GPIOB.10

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10

//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出

//  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.9

//

//  //USART1_RX	  GPIOB.11初始化

//  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB.11

//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入

//  GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOA.10

//  //Usart1 NVIC 配置

//  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;

//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3

//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3

//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能

//	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

//  //USART 初始化设置

//	USART_InitStructure.USART_BaudRate = bound;//串口波特率

//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式

//	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位

//	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位

//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制

//	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

//  USART_Init(USART3, &USART_InitStructure); //初始化串口1

//  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断

//  USART_Cmd(USART3, ENABLE);                    //使能串口1

//}

//void USART3_IRQHandler(void)                	//串口3中断服务程序

//{

//	u8 Res;

//	static u8 str=0;

//#if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.

//	OSIntEnter();

//#endif

//	if(USART_GetITStatus(USART3, USART_IT_RXNE))  //接收中断

//	{

//		Res = USART_ReceiveData(USART3);	//读取接收到的数据

//		if(Res=='R')					//以'R'开头

//		{

//			str=0;

//			USART_RX_BUF3[str] = Res;

//			USART_RX_STA3=1;

//		}

//		else if(USART_RX_STA3==1)

//		{

//			str ++;

//			USART_RX_BUF3[str] = Res;

//			if(Res=='!')				//以'!'结尾

//			{

//				USART_RX_STA3 = 100;

//			}

//		}

//  }

//#if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.

//	OSIntExit();

//#endif

//}

//#endif

//u16 distance=0;

//void runUSART3Command()			//摄像头信息解析函数

//{

//	if(USART_RX_STA3 ==100)

//	{

//		char chx=0,chx2=0,i,ddd[20];

//		static int ang=30;

//		float angle=0;

//		int num = 255;

//		chx = USART_RX_BUF3[1];

//		chx2=	USART_RX_BUF3[2];

//		USART_RX_STA3=0;

//		if(chx=='C')

//		{

//			if(chx2=='S');				//succeed

//			else	if(chx2=='F')		//Failure    需重新校准

//			{

//				sprintf(ddd,"%s%d%c","AC",num,'!');

//				senddata(ddd,strlen(ddd));

//			}

//		}

//		else if(chx=='A')

//		{

//			for(i=3;i<10;i++)

//			{

//				if(USART_RX_BUF3[i]=='!')   break ;

//				else angle = angle * 10 + USART_RX_BUF3[i]-0X30;

//			}

//			if(chx2=='L'||chx2=='R')

//			{

//				if(chx2=='L')

//					angle = angle * - 1;

//				servo(4,angle,0);		//调用舵机控制函数  水平调整

//				sprintf(ddd,"%s","AA!");

//				senddata(ddd,strlen(ddd));

//			}

//			else if(chx2=='N')			//Failure  不在范围内

//			{

//				if(ang>30)

//				{

//					ang = -30;

//					servo(4,0,ang);

//				}

//				else

//					servo(4,ang,0);		//调用舵机控制函数  水平调整

//				ang += 12;

//				sprintf(ddd,"%s","AA!");

//				senddata(ddd,strlen(ddd));

//			}

//			else if(chx2=='S')			//succeed		发送获取距离命令

//			{

//				sprintf(ddd,"%s","AD!");

//				senddata(ddd,strlen(ddd));

//				return ;

//			}

//			else			//Failure	需重新获取角度

//			{

//				sprintf(ddd,"%s","AA!");

//				senddata(ddd,strlen(ddd));

//			}

//		}

//		else if(chx=='D')			//获得摄像头的距离

//		{

//			num = 0;

//			for(i=2;i<10;i++)

//			{

//				if(USART_RX_BUF3[i]=='!')   break;

//				else num = num * 10 + USART_RX_BUF3[i]-0X30;

//			}

//			distance = num;   //计算距离赋值给外部变量

//		}

//	}

//}

//void senddata(char *p,char n)//往串口3发送字符串，指定个数

//{

//   if(p == 0) return ;

//	 if(n > 0)

//	 {

//		 while(n --)

//		 {

//			 USART_SendData(USART3,*p);		//向串口1发送数据

//			 delay_us(90);   							//115200不能低于90us的延时

//			 p++;

//			 USART_GetFlagStatus(USART3,USART_FLAG_TC);//等待发送结束

//		 }

//	 }

//}

//#endif

///*

//		sprintf(ddd,"%s%d%c","AC",num,'!');			//发送校准命令  num为摄像头距离引导标识的距离

//		sprintf(ddd,"%s","AA!");						//发送角度命令

//		sprintf(ddd,"%s","AD!");						//发送获取距离命令

//		senddata(ddd,strlen(ddd));

//*/
