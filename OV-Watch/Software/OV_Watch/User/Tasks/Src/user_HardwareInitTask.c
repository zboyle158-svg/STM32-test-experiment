/**
* @file user_HardwareInitTask.c
* @brief OV-Watch硬件和LVGL的一次性初始化任务。
* @details
* 该任务在FreeRTOS启动后执行外设、传感器、存储器、显示和输入设备初始化。
* 初始化完成后恢复任务调度并删除自身。
*/

/* Private includes -----------------------------------------------------------*/

// includes
// sys
#include "usart.h"
#include "tim.h"
#include "stm32f4xx_it.h"
#include "delay.h"

// user
#include "user_TasksInit.h"
#include "HWDataAccess.h"
#include "version.h"

// bsp
#include "key.h"
#include "lcd.h"
#include "lcd_init.h"
#include "CST816.h"
#include "DataSave.h"

// ui
//gui
#include "lvgl.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "ui.h"

// APP SYS setting
#include "ui_DateTimeSetPage.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


/**
* @brief 初始化OV-Watch硬件、LVGL显示和触摸输入。
* @param argument 未使用的任务参数。
* @details
* 初始化期间暂时挂起任务调度，避免其他业务任务在硬件尚未准备完成时运行。
* 所有初始化完成后恢复调度，并删除当前一次性任务。
*/
void HardwareInitTask(void *argument)
{
    while(1)
    {
        /**
        * @brief 初始化阶段暂时停止任务切换。
        * @note 中断仍可运行；该操作只阻止其他RTOS任务抢占初始化流程。
        */
        vTaskSuspendAll();

        /** @brief 配置RTC周期唤醒，用于低功耗模式恢复。 */
        if(HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 2000, RTC_WAKEUPCLOCK_RTCCLK_DIV16) != HAL_OK)
        {
            Error_Handler();
        }
        /** @brief 启动蓝牙串口DMA接收和UART空闲检测。 */
        HAL_UART_Receive_DMA(&huart1,(uint8_t*)HardInt_receive_str,25);
        __HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);

        /** @brief 启动TIM3 PWM输出。 */
        HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_3);

        /** @brief 初始化微秒/毫秒延时模块。 */
        delay_init();
        // wait
        // delay_ms(1000);

        /** @brief 初始化电源控制和电池检测接口。 */
        HWInterface.Power.Init();

        /** @brief 初始化按键输入和按键中断。 */
        Key_Port_Init();

        /**
        * @brief 初始化传感器。
        * @details 每个传感器最多尝试3次；初始化失败状态保存在对应ConnectionError字段。
        */
        uint8_t num = 3;
        while(num && HWInterface.AHT21.ConnectionError)
        {
            num--;
            HWInterface.AHT21.ConnectionError = HWInterface.AHT21.Init();
        }

        num = 3;
        while(num && HWInterface.Ecompass.ConnectionError)
        {
            num--;
            HWInterface.Ecompass.ConnectionError = HWInterface.Ecompass.Init();
        }
        if(!HWInterface.Ecompass.ConnectionError)
        HWInterface.Ecompass.Sleep();

        num = 3;
        while(num && HWInterface.Barometer.ConnectionError)
        {
            num--;
            HWInterface.Barometer.ConnectionError = HWInterface.Barometer.Init();
        }

        num = 3;
        while(num && HWInterface.IMU.ConnectionError)
        {
            num--;
            HWInterface.IMU.ConnectionError = HWInterface.IMU.Init();
            // Sensor_MPU_Erro = MPU_Init();
        }

        num = 3;
        while(num && HWInterface.HR_meter.ConnectionError)
        {
            num--;
            HWInterface.HR_meter.ConnectionError = HWInterface.HR_meter.Init();
        }
        if(!HWInterface.HR_meter.ConnectionError)
        HWInterface.HR_meter.Sleep();


        /** @brief 初始化EEPROM并恢复用户设置和当日步数。 */
        EEPROM_Init();
        if(!EEPROM_Check())
        {
            uint8_t recbuf[3];
            SettingGet(recbuf,0x10,2);
            if((recbuf[0]!=0 && recbuf[0]!=1) || (recbuf[1]!=0 && recbuf[1]!=1))
            {
                HWInterface.IMU.wrist_is_enabled = 0;
                ui_APPSy_EN = 0;
            }
            else
            {
                HWInterface.IMU.wrist_is_enabled = recbuf[0];
                ui_APPSy_EN = recbuf[1];
            }

            RTC_DateTypeDef nowdate;
            HAL_RTC_GetDate(&hrtc,&nowdate,RTC_FORMAT_BIN);

            SettingGet(recbuf,0x20,3);
            if(recbuf[0] == nowdate.Date)
            {
                uint16_t steps=0;
                steps = recbuf[1]&0x00ff;
                steps = steps<<8 | recbuf[2];
                if(!HWInterface.IMU.ConnectionError)
                dmp_set_pedometer_step_count((unsigned long)steps);
            }
        }


        /** @brief 初始化蓝牙模块，启动后默认关闭蓝牙以降低功耗。 */
        HWInterface.BLE.Init();
        HWInterface.BLE.Disable();

        //set the KT6328 BautRate 9600
        //default is 115200
        //printf("AT+CT01\r\n");

        /** @brief 初始化CST816触摸控制器并执行复位。 */
        CST816_GPIO_Init();
        CST816_RESET();

        /**
        * @brief 初始化LCD并显示启动画面。
        * @details 启动画面包含当前软件版本号，随后清除启动画面区域。
        */
        LCD_Init();
        LCD_Fill(0,0, LCD_W, LCD_H, BLACK);
        delay_ms(10);
        LCD_Set_Light(50);
        LCD_ShowString(72,LCD_H/2,(uint8_t*)"Welcome!", WHITE, BLACK, 24, 0);//12*6,16*8,24*12,32*16
        uint8_t lcd_buf_str[17];
        sprintf(lcd_buf_str, "OV-Watch V%d.%d.%d", watch_version_major(), watch_version_minor(), watch_version_patch());
        LCD_ShowString(34, LCD_H/2+48, (uint8_t*)lcd_buf_str, WHITE, BLACK, 24, 0);
        delay_ms(1000);
        LCD_Fill(0, LCD_H/2-24, LCD_W, LCD_H/2+49, BLACK);


        /**
        * @brief 初始化LVGL、显示端口、输入端口和页面对象。
        */
        lv_init();
        lv_port_disp_init();
        lv_port_indev_init();
        ui_init();

        xTaskResumeAll();
        vTaskDelete(NULL);
        osDelay(500);
    }
}


