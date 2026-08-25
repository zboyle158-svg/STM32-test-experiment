/**
* @file user_ChargCheckTask.c
* @brief OV-Watch FreeRTOS任务与任务间通信实现。
* @details
* 本文件仅实现本模块职责；底层硬件访问通过对应HAL/BSP接口完成。
* 修改本文件时应保持公开接口、返回值约定、时序和硬件引脚配置不变。
*/

/* Private includes -----------------------------------------------------------*/
//includes
#include "user_TasksInit.h"
#include "user_ScrRenewTask.h"
#include "user_RunModeTasks.h"
#include "ui_HomePage.h"
#include "ui_ChargPage.h"
#include "main.h"
#include "HWDataAccess.h"
#include "stm32f4xx_it.h"
/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/


/**
* @brief  charg page enter task
* @param  argument: Not used
* @retval None
*/
void ChargPageEnterTask(void *argument)
{
    while(1)
    {
        uint32_t hardint_flags = osEventFlagsWait(HardIntEventHandle, HARDINT_EVENT_CHARG, osFlagsWaitAny, osWaitForever);
        if((hardint_flags & HARDINT_EVENT_CHARG) != 0U)
        {
            uint8_t IdleBreakstr = 0;
            osMessageQueuePut(IdleBreak_MessageQueue,&IdleBreakstr,NULL,1);
            if((ChargeCheck()) && (Page_Get_NowPage()->page_obj != &ui_ChargPage))
            {
                Page_Load(&Page_Charg);
            }
            else if((!ChargeCheck()) && (Page_Get_NowPage()->page_obj == &ui_ChargPage))
            {
                Page_Back();
            }
        }
    }
}

