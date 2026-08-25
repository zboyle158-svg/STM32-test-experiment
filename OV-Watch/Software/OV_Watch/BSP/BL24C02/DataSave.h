/**
* @file DataSave.h
* @brief OV-Watch板级外设驱动实现。
* @details
* 本文件仅实现本模块职责；底层硬件访问通过对应HAL/BSP接口完成。
* 修改本文件时应保持公开接口、返回值约定、时序和硬件引脚配置不变。
*/

#ifndef __DATASAVE_H__
#define __DATASAVE_H__

#include "BL24C02.h"

void EEPROM_Init(void);
uint8_t EEPROM_Check(void);
uint8_t SettingSave(uint8_t *buf, uint8_t addr, uint8_t lenth);
uint8_t SettingGet(uint8_t *buf, uint8_t addr, uint8_t lenth);

#endif
