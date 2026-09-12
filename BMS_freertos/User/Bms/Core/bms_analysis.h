/** @file bms_analysis.h
 *  @brief BMS 状态分析、容量和 SOC 接口。
 *  @details 声明分析结果结构体及分析任务初始化函数；结果由 bms_analysis.c 周期更新，其他任务只读。
 *  @par 使用方法 先调用 BMS_AnalysisInit() 创建任务，再读取 BMS_AnalysisData。
 */
#ifndef __BMS_ANALYSIS_H__
#define __BMS_ANALYSIS_H__


#include <stdint.h>




typedef struct
{
	uint8_t SOH;	// 电池健康状态，通常以百分比表示。
	uint8_t SOP;	// 电池可用功率状态，通常以百分比表示。
	uint8_t SOE;	// 电池能量状态，通常以百分比表示。
	uint32_t LoopCount;			// 完成的充放电循环次数。
	float CapacityLoop;			// 当前循环累计容量，单位为Ah。
	float SOC;					// 荷电状态，通常范围为0.0到1.0。

	float AverageVoltage;		// 电芯平均电压，单位为V。
	float MaxVoltageDifference;	// 电芯最大压差，单位为V。
	float PowerReal;			// 当前实际功率，单位为W。
	float CellVoltMax;			// 当前最高电芯电压，单位为V。
	float CellVoltMin;			// 当前最低电芯电压，单位为V。
	
	float CapacityRated;		// 电池标称容量，单位为Ah。
	float CapacityReal;			// 当前实际可用容量，单位为Ah。
	float CapacityRemain;		// 当前剩余容量，单位为Ah。
}BMS_AnalysisDataTypedef;


extern BMS_AnalysisDataTypedef BMS_AnalysisData;



void BMS_AnalysisInit(void);



#endif

