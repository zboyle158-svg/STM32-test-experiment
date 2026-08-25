/**
 * @file    HARDWARE/user/user.c
 * @brief   电磁曲线炮应用控制模块。
 *
 * @details 本文件封装本模块的硬件初始化、数据处理和对外接口。
 *          具体引脚、通信参数和时序以当前实现及工程配置为准。
 */

#include "user.h"

#include <math.h>

#include "HMI.h"

#include "servo.h"

#include "laser_ranging.h"

void initialize(void)  //初始化发射光耦、继电器和蜂鸣器使用的 GPIO

{

	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE,ENABLE); //按原工程一次开启 GPIOA～GPIOE 时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	 //端口配置

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz

	GPIO_Init(GPIOA, &GPIO_InitStructure);					 		 //根据设定参数初始化GPIOB.5

	GPIO_ResetBits(GPIOA,GPIO_Pin_6);					//可控硅	//拉低

	GPIO_SetBits(GPIOA,GPIO_Pin_7);					//继电器

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	 //端口配置

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 		 //推挽输出

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz

	GPIO_Init(GPIOA, &GPIO_InitStructure);					 		 //根据设定参数初始化GPIOB.5

	GPIO_SetBits(GPIOA,GPIO_Pin_12);					//蜂鸣器

}

// 一次完整的手动发射时序：抬炮、等待、切换继电器/光耦、发射并恢复舵机。
// 函数包含长时间阻塞延时，执行期间主循环不会处理新的触摸屏命令。

void shoot(void)

{

	int i=1;

//	servo(3,35);		//舵机抬炮

/********不可动程序*************/

	k1=1; //打开继电器

	for(;i<70;i++)

	{

		servo(3,i);		//舵机抬炮

		delay_ms(10);

	}

	for(;i>3;i--)

	{

		servo(3,i);		//舵机抬炮

		delay_ms(10);

	}

	delay_ms(1500);

	delay_ms(1500);

	delay_ms(1500);

	k1=0;	delay_ms(500);

	Open_BEEP;

	k0=1;	//关闭继电器,开始发射

	TIM_SetCompare2(TIM5,0);

	delay_ms(500);

	Close_BEEP;

	k0=0;			//结束

	k1=1;

/*******************************/

	delay_ms(100);

	servo(2,0);		//复位舵机

}

// 指定距离发射：用项目标定公式把厘米距离换算为TIM5比较值。

void tance_1(float tance)

{

	float	pwm =0;

	pwm = (tance + 1.074*100) / (0.1544 * 100) * 10;		//单位为 CM

	TIM_SetCompare2(TIM5,(int)pwm);

	shoot();

}

void tance_2(float tance,float angl)    //设置目标距离和水平角度后发射

{

	float	pwm =0;

	pwm = (tance + 1.074*100) / (0.1544 * 100) * 10;		//单位为 CM

	TIM_SetCompare2(TIM5,(int)pwm);

	servo(4,angl);		//调用舵机控制函数

	shoot();

}

char dddd[10];

int flage = 330;	//激光距离比较值

int fan=30;				//激光扫描范围

int tit=0;				//激光寻靶距离补偿

// 自动寻靶：水平扫描并记录距离阈值两侧的角度，最后取平均角度发射。

void tance_3(char t)    //按扫描方向自动寻找目标并发射

{

	unsigned char a=5,b=1;

	unsigned char bitt = 2;

	float ang1 = 0,ang2 = 0;

	float num = 0,bu=0;		//角度补偿

	float	pwm =0;

	int distance =0;

	int i=0;

	k1=1; //打开继电器

	TIM_SetCompare2(TIM5,900);

	for(;i<70;i++)

	{

		servo(3,i);		//舵机抬炮

		delay_ms(10);

	}

	for(;i>3;i--)

	{

		servo(3,i);		//舵机抬炮

		delay_ms(10);

	}

	servo(3,0);		//舵机抬炮

	if(t==2)				//从右往左

	{

		bu=-1;

		num = -fan;

		servo(4,-fan);		//水平角度到达-37度

	}

	else						//从左往右

	{

		bu=3;

		num = fan;

		servo(4,fan);		//水平角度到达37度

	}

	delay_ms(300);

	do

	{

		if(USART_RX_STA3==100)

		{

			USART_RX_STA3=0;

			distance = ranging();

			if(distance > 50)

			{

				if(distance < flage && bitt==2)				//超声波距离比较值,初值为 330 CM

				{

					ang1 = angle;

					if(a > 0) a--;

					else if(a == 0)

					{

						k1=0;

						servo(3,4);						//垂直角度复位

						a = 5;

						bitt=1;

						delay_ms(100);

						Open_BEEP;

						k0=1;	//关闭继电器,开始发射

						sprintf(dddd,"t2.txt=\"%d %s\"",distance,"cm");

						sendStringHMI(dddd);

						TIM_SetCompare2(TIM5,0);

						delay_ms(200);

						servo(3,0);						//垂直角度复位

						Close_BEEP;

						k0=0;			//结束

						k1=1;

					}

				}

				else if(distance > flage && bitt == 1)	//超声波距离比较值,初值为 330 CM

				{

					ang2 = angle;

					if(b == 1) b = 0;

					else if(b == 0) bitt = 2;

				}

				if(t==2)

				{

					if(num >fan)		num = - fan;

					servo(4,num);

					num +=0.3;

				}

				else

				{

					if(num < -fan)		num = fan;

					servo(4,num);

					num -=0.3;

				}

			}

		}

	}

	while(bitt);

	k1=0;

	servo(3,4);						//垂直角度复位

	num = (ang1 + ang2) / 2 + bu;

	servo(4,num);

	Open_BEEP;

	delay_ms(300);

	//delay_ms(1000);

	//delay_ms(1000);

//	if(USART_RX_STA3==100)

//	{

//		USART_RX_STA3 = 0;

//		distance = ranging();

//	}

	k0=1;	//关闭继电器,开始发射

	sprintf(dddd,"t2.txt=\"%d %s\"",distance,"cm");

	sendStringHMI(dddd);

	TIM_SetCompare2(TIM5,0);

	delay_ms(200);

	servo(3,0);						//垂直角度复位

	Close_BEEP;

	k0=0;			//结束

	k1=1;

//	for(int i=0 ; i<20; i++)

//	{

//		servo(4,num++);		//舵机抬炮

//		delay_ms(200);

//	}

//	delay_ms(200);

//	servo(2,0);		//复位舵机

	//pwm = (distance-25 + tit + 1.074*100) / (0.1544 * 100) * 10;		//单位为 CM

	//pwm = 900;

	//if((((ang1 + ang2) / 2 + bu))>0)	pwm += 3;

	//TIM_SetCompare2(TIM5,(int)pwm);

	//shoot();

}
