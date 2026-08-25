/**
 * @file    HARDWARE/ADC/adc.c
 * @brief   ADC 模拟量采集模块。
 *
 * @details 本文件封装本模块的硬件初始化、数据处理和对外接口。
 *          具体引脚、通信参数和时序以当前实现及工程配置为准。
 */

#include "adc.h"

#include "delay.h"

//初始化ADC

//这里我们仅以规则通道为例

//我们默认将开启通道0~3

void Adc_Init(void)

{

	ADC_InitTypeDef ADC_InitStructure;

	GPIO_InitTypeDef GPIO_InitStructure;

																						//|RCC_APB2Periph_ADC2

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_ADC1,ENABLE );	  //使能ADC1通道时钟   2

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M      3

	//PA1 作为模拟通道输入引脚

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7;   //GPIOA _Pin_1

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//模拟输入引脚

	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_DeInit(ADC1);			//复位ADC1    4

//	ADC_DeInit(ADC2);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1工作在独立模式

	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式

	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式

	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动

	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐

	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目

	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器      5

//	ADC_Init(ADC2, &ADC_InitStructure);

	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1    6

//	ADC_Cmd(ADC2, ENABLE);

	ADC_ResetCalibration(ADC1);	//使能复位校准

//	ADC_ResetCalibration(ADC2);

	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束

//	while(ADC_GetResetCalibrationStatus(ADC2));

	ADC_StartCalibration(ADC1);	 //开启AD校准

//	ADC_StartCalibration(ADC2);

	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束

//	while(ADC_GetCalibrationStatus(ADC2));

//	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

}

float Get_Adc_Average(u8 ch, u8 times)		//获得ADC值

{

  float temp_val=0;

	u8 t;

	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_71Cycles5 );	//ADC1,ADC通道,采样时间为71.5周期	  7

	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	   8

	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	for(t=0;t<times;t++)

	{

		temp_val += ADC_GetConversionValue(ADC1);  //最近一次ADC1规则组的转换结果

		delay_ms(8);

	}

	temp_val = temp_val / times * ( 2.492 / 4096)  *150 ;  //需要加衰减电路

	return temp_val;

}
