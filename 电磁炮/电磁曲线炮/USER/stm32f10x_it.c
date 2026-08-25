/**
  ******************************************************************************
  * @file    USER/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Cortex-M3 异常处理函数。
  *          本文件提供系统异常入口；外设中断实现位于对应 HARDWARE 模块。
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"

/*
 * Cortex-M3 异常处理函数
 *
 * 这些函数名必须与启动文件中的中断向量表保持一致。当前工程没有在
 * 这些异常入口中增加业务逻辑；发生不可恢复异常时保留死循环，便于
 * 在调试器中停留并定位故障现场。
 */

void NMI_Handler(void)
{
    /* NMI 通常用于不可屏蔽的系统级事件；本工程没有配置 NMI 外设源。 */
}

void HardFault_Handler(void)
{
    /* 硬 Fault 表示发生了无法恢复的严重错误，例如非法访问或执行错误。 */
    while (1)
    {
    }
}

void MemManage_Handler(void)
{
    /* Cortex-M3 未启用 MPU 时通常不会进入这里；保留死循环便于调试定位。 */
    while (1)
    {
    }
}

void BusFault_Handler(void)
{
    /* 总线访问错误通常与非法地址、外设访问时序或存储器故障有关。 */
    while (1)
    {
    }
}

void UsageFault_Handler(void)
{
    /* 指令或处理器使用错误，例如未对齐访问或执行未定义指令。 */
    while (1)
    {
    }
}

void SVC_Handler(void)
{
    /* 本工程未使用 SVC；若以后引入 RTOS，应在这里接入系统调用处理。 */
}

void DebugMon_Handler(void)
{
    /* 调试监视异常入口；未配置 DebugMonitor 时保持空处理函数。 */
}

void PendSV_Handler(void)
{
    /* PendSV 常用于 RTOS 上下文切换；本工程未使用 RTOS。 */
}

void SysTick_Handler(void)
{
    /* 当前 SYSTEM_SUPPORT_OS 为 0，delay.c 使用 SysTick 轮询完成阻塞延时，
       因此这里不再实现 SysTick 中断逻辑，避免与 delay.c 的时间基准冲突。 */
}

/*
 * 外设中断说明
 *
 * USART1_IRQHandler 和 USART3_IRQHandler 的实际实现位于对应的 HARDWARE
 * 模块中。本文件只保留 Cortex-M3 异常入口，避免同名函数重复定义。
 * 启动文件中的向量表会根据函数名把中断请求分发到相应的处理函数。
 */
