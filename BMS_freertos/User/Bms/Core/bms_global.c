/** @file bms_global.c
 *  @brief BMS 全局运行状态的唯一实例定义。
 *  @details 提供系统模式、充放电状态、均衡状态及实际串数的初始值。
 */
#include "bms_global.h"




BMS_GlobalParamTypedef BMS_GlobalParam = 
{
	.SysMode 			= BMS_MODE_STANDBY,
	.Cell_Real_Number 	= BMS_CELL_MAX,
	.Temp_Real_Number 	= BMS_TEMP_MAX,
	.Charge 			= BMS_STATE_DISABLE,
	.Discharge 			= BMS_STATE_DISABLE,
	.Balance		 	= BMS_STATE_DISABLE,
};



