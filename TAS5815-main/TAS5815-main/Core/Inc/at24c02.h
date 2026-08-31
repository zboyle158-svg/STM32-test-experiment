/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    at24c02.h
  * @brief   AT24C02 EEPROM驱动头文件
  ******************************************************************************
  * @attention
  * 
  * 功能说明：
  * - I2C接口EEPROM，容量2Kbit (256字节)
  * - 设备地址：0x50 (7-bit)
  * - 页写入大小：8字节
  * - 用于存储音量等配置参数
  * 
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef __AT24C02_H
#define __AT24C02_H

#include "main.h"
#include <stdint.h>

/* AT24C02 设备地址 (7-bit) */
/** @brief AT24C02的7位I2C地址；调用HAL时需要左移一位。 */
#define AT24C02_ADDR        0x50

/* 音量存储地址 */
#define AT24C02_VOLUME_ADDR 0x00

/* 音量有效标志地址 */
#define AT24C02_FLAG_ADDR   0x01

/* 音量有效标志值 */
#define AT24C02_VALID_FLAG  0xA5

/* 电压设置存储地址 */
#define AT24C02_VOLTAGE_ADDR        0x02
#define AT24C02_VOLTAGE_FLAG_ADDR   0x03
#define AT24C02_VOLTAGE_VALID_FLAG  0x5A

/* 调制模式存储地址 */
#define AT24C02_MODULATION_ADDR        0x04
#define AT24C02_MODULATION_FLAG_ADDR   0x05
#define AT24C02_MODULATION_VALID_FLAG  0x3C

/* 开关频率存储地址 */
#define AT24C02_FSW_ADDR        0x06
#define AT24C02_FSW_FLAG_ADDR   0x07
#define AT24C02_FSW_VALID_FLAG  0xC3

/* 关机设置存储地址 */
#define AT24C02_POWEROFF_ADDR        0x08
#define AT24C02_POWEROFF_FLAG_ADDR   0x09
#define AT24C02_POWEROFF_VALID_FLAG  0xD7

/* AMP增益存储地址 */
#define AT24C02_AMP_ADDR        0x0A
#define AT24C02_AMP_FLAG_ADDR   0x0B
#define AT24C02_AMP_VALID_FLAG  0xE1

/**
 * @brief 检测AT24C02是否响应I2C地址。
 * @return 0表示设备就绪，1表示检测失败。
 * @note 该函数只检查I2C设备应答，不会修改EEPROM内容。
 */
uint8_t AT24C02_Init(void);

/**
 * @brief 向EEPROM内部地址写入一个字节。
 * @param addr EEPROM内部地址，范围0~255。
 * @param data 要写入的数据字节。
 * @return 0表示成功，1表示I2C传输失败。
 * @note 写操作结束后需要等待器件内部写周期完成。
 */
uint8_t AT24C02_WriteByte(uint8_t addr, uint8_t data);

/**
 * @brief 从EEPROM内部地址读取一个字节。
 * @param addr EEPROM内部地址，范围0~255。
 * @param data 输出参数，成功时写入读取结果。
 * @return 0表示成功，1表示I2C传输失败。
 */
uint8_t AT24C02_ReadByte(uint8_t addr, uint8_t *data);

/**
 * @brief 保存音量和对应有效标志。
 * @param volume 音量等级，当前应用范围0~99。
 * @return 0表示写入成功，1表示写入失败。
 * @note 函数会写入两个EEPROM字节，调用方应避免在I2C3被其他代码占用时调用。
 */
uint8_t AT24C02_SaveVolume(uint8_t volume);

/**
 * @brief 读取并校验音量。
 * @param volume 输出参数，成功时写入有效音量。
 * @return 0表示有效，1表示无效或读取失败。
 */
uint8_t AT24C02_LoadVolume(uint8_t *volume);

/**
 * @brief 保存电压档位及有效标志。
 * @param voltage 档位编码，0=5V，1=12V，2=高压档。
 * @return 0表示写入成功，1表示写入失败。
 */
uint8_t AT24C02_SaveVoltage(uint8_t voltage);

/**
 * @brief 读取并校验电压档位。
 * @param voltage 输出参数，成功时写入0~2的电压档位编码。
 * @return 0表示有效，1表示无效或读取失败。
 */
uint8_t AT24C02_LoadVoltage(uint8_t *voltage);

/** @brief 保存调制模式及有效标志。 */
uint8_t AT24C02_SaveModulation(uint8_t modulation);
/** @brief 读取并校验调制模式。 */
uint8_t AT24C02_LoadModulation(uint8_t *modulation);

/** @brief 保存功率级开关频率选择。 */
uint8_t AT24C02_SaveFSW(uint8_t fsw);
/** @brief 读取并校验功率级开关频率选择。 */
uint8_t AT24C02_LoadFSW(uint8_t *fsw);

/** @brief 保存自动关机模式。 */
uint8_t AT24C02_SavePowerOff(uint8_t mode);
/** @brief 读取并校验自动关机模式。 */
uint8_t AT24C02_LoadPowerOff(uint8_t *mode);

/** @brief 保存TAS5815模拟增益索引。 */
uint8_t AT24C02_SaveAMP(uint8_t gain);
/** @brief 读取并校验TAS5815模拟增益索引。 */
uint8_t AT24C02_LoadAMP(uint8_t *gain);

#endif /* __AT24C02_H */
