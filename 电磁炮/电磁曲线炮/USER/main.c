/**
 * @file    main.c
 * @brief   电磁曲线炮应用程序入口。
 *
 * @details
 * 完成板级外设初始化后，主循环持续解析触摸屏命令。串口接收和数据组帧
 * 由对应的串口中断服务函数完成，主循环只负责调用应用层解析接口。
 */

#include "stm32f10x.h"
#include "delay.h"          /* 延时函数；单次延时上限由模块实现限制。 */
#include "led.h"            /* LED 指示灯。 */
#include "key.h"            /* 按键输入。 */
#include "HMI.h"            /* 触摸屏通信与命令解析。 */
#include "servo.h"          /* 舵机控制。 */
#include "user.h"           /* 用户自定义 I/O。 */
#include "TIM5_PWM.h"       /* 定时器 PWM 输出。 */
#include "laser_ranging.h"  /* 激光测距模块通信。 */

/**
 * @brief  应用程序入口。
 * @return 不返回；系统在无限循环中持续处理触摸屏命令。
 */
int main(void)
{
    /* 先初始化用户 I/O，保持该工程原有的初始化顺序。 */
    initialize();

    /* 初始化 delay 模块，为后续外设和应用代码提供延时基础。 */
    delay_init();

    /*
     * 设置 NVIC 优先级分组 4：4 位抢占优先级、0 位响应优先级。
     * 具体中断优先级由各外设模块分别配置。
     */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

    /* 初始化两个串口：USART1 连接串口屏，USART3 连接激光测距模块。 */
    uart_init1(115200);
    uart_init3(115200);

    /* 初始化板载 LED 和按键输入。 */
    LED_Init();
    KEY_Init();

    /*
     * 初始化舵机 PWM：周期参数 19999、分频参数 71，对应约 20 ms 周期。
     * 初始化 TIM5 PWM：参数 999、71，对应约 1 ms 的定时基准。
     */
    TIM4_PWM_Init(19999, 71);
    TIM5_PWM_Init(999, 71);

    /*
     * 主循环只负责解析已经接收完成的触摸屏命令；串口收字节和组帧
     * 由对应的 USART 中断服务函数完成。
     */
    while (1)
    {
        runHMICommand();
    }
}
