/**
* @file user_KeyTask.c
* @brief OV-Watch FreeRTOS任务与任务间通信实现。
* @details
* 本文件仅实现本模块职责；底层硬件访问通过对应HAL/BSP接口完成。
* 修改本文件时应保持公开接口、返回值约定、时序和硬件引脚配置不变。
*/

/* Private includes -----------------------------------------------------------*/
//includes
#include "user_TasksInit.h"
#include "ui_HomePage.h"
#include "main.h"
#include "key.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


/**
* @brief  Key press check task
* @param  argument: Not used
* @retval None
*/
void KeyTask(void *argument)
{
    uint8_t keystr=0;
    uint8_t Stopstr=0;
    uint8_t IdleBreakstr=0;
    while(1)
    {
        switch(KeyScan(0))
        {
        case 1:
            keystr = 1;
            osMessageQueuePut(Key_MessageQueue, &keystr, 0, 1);
            osMessageQueuePut(IdleBreak_MessageQueue, &IdleBreakstr, 0, 1);
            break;

        case 2:
            if(Page_Get_NowPage()->page_obj == &ui_HomePage)
            {
                osMessageQueuePut(Stop_MessageQueue, &Stopstr, 0, 1);
            }
            else
            {
                keystr = 2;
                osMessageQueuePut(Key_MessageQueue, &keystr, 0, 1);
                osMessageQueuePut(IdleBreak_MessageQueue, &IdleBreakstr, 0, 1);
            }
            break;
        }
        osDelay(1);
    }
}
