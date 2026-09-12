/** @file bms_hal_config.c @brief BQ769x0 延时和阈值配置实现。 @details 将业务枚举转换为芯片寄存器编码。 @par 使用方法 由 BMS_SysInitialize() 调用。 */
#include "bms_hal_config.h"

#include "drv_softi2c_bq769x0.h"








void BMS_HalCfgSCDDelaySet(BMS_SCDDelayTypedef SCDDelay)
{
	BQ769X0_SCDDelaySet((BQ769X0_SCDDelayTypedef)SCDDelay);
}

void BMS_HalCfgOCDDelaySet(BMS_OCDDelayTypedef OCDDelay)
{
	BQ769X0_OCDDelaySet((BQ769X0_OCDDelayTypedef)OCDDelay);
}

void BMS_HalCfgUVDelaySet(BMS_UVDelayTypedef UVDelay)
{
	BQ769X0_UVDelaySet((BQ769X0_UVDelayTypedef)UVDelay);
}

void BMS_HalCfgOVDelaySet(BMS_OVDelayTypedef OVDelay)
{
	BQ769X0_OVDelaySet((BQ769X0_OVDelayTypedef)OVDelay);
}

void BMS_HalCfgUVPThresholdSet(uint16_t UVPThreshold)
{
	BQ769X0_UVPThresholdSet(UVPThreshold);
}

void BMS_HalCfgOVPThresholdSet(uint16_t OVPThreshold)
{
	BQ769X0_OVPThresholdSet(OVPThreshold);
}


