/**
 * @file    HARDWARE/user/user.h
 * @brief   电磁曲线炮应用控制模块接口。
 *
 * @details
 * 提供应用层硬件初始化、发射控制、自动模式、手动模式以及测距相关接口。
 * 本文件只声明接口和宏，不在头文件中执行实际控制流程。
 */

#ifndef __USER_H
#define __USER_H

#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"

/* 发射光耦、继电器和蜂鸣器使用的 GPIO 位带访问宏。 */
#define k0          PAout(6)        /* 发射光耦控制。 */
#define k1          PAout(7)        /* 继电器 1 控制。 */
#define BEEP        PAout(12)       /* 蜂鸣器控制。 */

/* 蜂鸣器为低电平有效：0 开启，1 关闭。 */
#define Open_BEEP   PAout(12) = 0
#define Close_BEEP  PAout(12) = 1

/**
 * @brief  初始化应用层使用的 GPIO 和相关控制资源。
 */
void initialize(void);

/**
 * @brief  执行一次发射控制流程。
 */
void shoot(void);

/**
 * @brief  执行自动寻靶和发射模式。
 */
void AutoMode(void);

/**
 * @brief  执行手动控制模式。
 */
void HandleMode(void);

/**
 * @brief  执行半自动手柄控制模式。
 */
void HalfHandleMode(void);

/**
 * @brief  根据测距结果执行第一种控制策略。
 * @param  tance 测得的距离或距离计算结果。
 */
void tance_1(float tance);

/**
 * @brief  根据距离和水平角度执行第二种控制策略。
 * @param  tance 测得的距离或距离计算结果。
 * @param  angle 目标水平方向角度。
 */
void tance_2(float tance, float angle);

/**
 * @brief  根据扫描方向执行第三种控制策略。
 * @param  t 扫描或控制方向标志。
 */
void tance_3(char t);

/* 应用层运行状态变量，由 user.c 定义并由其他模块读取。 */
extern int flage; /* 激光距离比较值。 */
extern int fan;   /* 激光扫描范围。 */
extern int tit;   /* 激光寻靶距离补偿值。 */

#endif /* __USER_H */
