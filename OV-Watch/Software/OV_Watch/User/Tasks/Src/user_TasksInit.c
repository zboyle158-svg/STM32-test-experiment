/**
* @file user_TasksInit.c
* @brief OV-Watch任务、消息队列和事件标志的集中创建入口。
* @details
* 本文件负责创建RTOS对象，不实现各任务的具体业务。
* 任务之间通过消息队列、事件标志和共享状态进行协作。
*/

/* Private includes -----------------------------------------------------------*/
//includes
#include "user_TasksInit.h"
//sys
#include "sys.h"
#include "stdio.h"
#include "lcd.h"
#include "WDOG.h"
//gui
#include "lvgl.h"
#include "ui_TimerPage.h"
//tasks
#include "user_HardwareInitTask.h"
#include "user_RunModeTasks.h"
#include "user_KeyTask.h"
#include "user_ScrRenewTask.h"
#include "user_SensUpdateTask.h"
#include "user_ChargCheckTask.h"
#include "user_MessageSendTask.h"
#include "user_DataSaveTask.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private variables ---------------------------------------------------------*/


/* Tasks ---------------------------------------------------------------------*/
/** @brief 硬件一次性初始化任务句柄和属性。 */
osThreadId_t HardwareInitTaskHandle;
const osThreadAttr_t HardwareInitTask_attributes = {
    .name = "HardwareInitTask",
    .stack_size = 128 * 10,
    .priority = (osPriority_t) osPriorityHigh3,
};

/** @brief LVGL后台处理任务句柄和属性。 */
osThreadId_t LvHandlerTaskHandle;
const osThreadAttr_t LvHandlerTask_attributes = {
    .name = "LvHandlerTask",
    .stack_size = 128 * 24,
    .priority = (osPriority_t) osPriorityLow,
};

/** @brief 外部看门狗喂狗任务句柄和属性。 */
osThreadId_t WDOGFeedTaskHandle;
const osThreadAttr_t WDOGFeedTask_attributes = {
    .name = "WDOGFeedTask",
    .stack_size = 128 * 1,
    .priority = (osPriority_t) osPriorityHigh2,
};

/** @brief 空闲背光和浅睡眠管理任务句柄和属性。 */
osThreadId_t IdleEnterTaskHandle;
const osThreadAttr_t IdleEnterTask_attributes = {
    .name = "IdleEnterTask",
    .stack_size = 128 * 1,
    .priority = (osPriority_t) osPriorityHigh,
};

/** @brief STOP低功耗进入和唤醒恢复任务句柄和属性。 */
osThreadId_t StopEnterTaskHandle;
const osThreadAttr_t StopEnterTask_attributes = {
    .name = "StopEnterTask",
    .stack_size = 128 * 16,
    .priority = (osPriority_t) osPriorityHigh1,
};

/** @brief 物理按键扫描任务句柄和属性。 */
osThreadId_t KeyTaskHandle;
const osThreadAttr_t KeyTask_attributes = {
    .name = "KeyTask",
    .stack_size = 128 * 1,
    .priority = (osPriority_t) osPriorityNormal,
};

/** @brief 页面返回和页面切换任务句柄和属性。 */
osThreadId_t ScrRenewTaskHandle;
const osThreadAttr_t ScrRenewTask_attributes = {
    .name = "ScrRenewTask",
    .stack_size = 128 * 10,
    .priority = (osPriority_t) osPriorityLow1,
};

/** @brief 普通传感器数据更新任务句柄和属性。 */
osThreadId_t SensorDataTaskHandle;
const osThreadAttr_t SensorDataTask_attributes = {
    .name = "SensorDataTask",
    .stack_size = 128 * 5,
    .priority = (osPriority_t) osPriorityLow1,
};

/** @brief 心率血氧数据更新任务句柄和属性。 */
osThreadId_t HRDataTaskHandle;
const osThreadAttr_t HRDataTask_attributes = {
    .name = "HRDataTask",
    .stack_size = 128 * 5,
    .priority = (osPriority_t) osPriorityLow1,
};

/** @brief 充电状态检测和充电页面管理任务句柄和属性。 */
osThreadId_t ChargPageEnterTaskHandle;
const osThreadAttr_t ChargPageEnterTask_attributes = {
    .name = "ChargPageEnterTask",
    .stack_size = 128 * 10,
    .priority = (osPriority_t) osPriorityLow1,
};

/** @brief 蓝牙串口命令接收和响应任务句柄和属性。 */
osThreadId_t MessageSendTaskHandle;
const osThreadAttr_t MessageSendTask_attributes = {
    .name = "MessageSendTask",
    .stack_size = 128 * 5,
    .priority = (osPriority_t) osPriorityLow1,
};

/** @brief MPU6050姿态和抬腕状态检测任务句柄和属性。 */
osThreadId_t MPUCheckTaskHandle;
const osThreadAttr_t MPUCheckTask_attributes = {
    .name = "MPUCheckTask",
    .stack_size = 128 * 3,
    .priority = (osPriority_t) osPriorityLow2,
};

/** @brief EEPROM数据保存任务句柄和属性。 */
osThreadId_t DataSaveTaskHandle;
const osThreadAttr_t DataSaveTask_attributes = {
    .name = "DataSaveTask",
    .stack_size = 128 * 5,
    .priority = (osPriority_t) osPriorityLow2,
};


/* Message queues ------------------------------------------------------------*/
/** @brief 按键事件队列及其他任务通信对象。 */
osMessageQueueId_t Key_MessageQueue;
osMessageQueueId_t Idle_MessageQueue;
osMessageQueueId_t Stop_MessageQueue;
osMessageQueueId_t IdleBreak_MessageQueue;
osMessageQueueId_t HomeUpdata_MessageQueue;
osMessageQueueId_t DataSave_MessageQueue;
osEventFlagsId_t HardIntEventHandle;

/* Private function prototypes -----------------------------------------------*/
void LvHandlerTask(void *argument);
void WDOGFeedTask(void *argument);

/**
* @brief 创建OV-Watch应用任务和通信对象。
* @details
* 创建顺序不等于执行顺序；任务要等osKernelStart启动调度后，
* 再由优先级和阻塞状态共同决定运行时机。
*/
void User_Tasks_Init(void)
{
    /* add mutexes, ... */

    /* add semaphores, ... */

    /* start timers, add new ones, ... */

    /**
    * @brief 创建任务间消息队列和事件标志。
    * @note 队列元素大小为1字节，主要传递事件编号。
    */
    Key_MessageQueue  = osMessageQueueNew(1, 1, NULL);
    Idle_MessageQueue = osMessageQueueNew(1, 1, NULL);
    Stop_MessageQueue = osMessageQueueNew(1, 1, NULL);
    IdleBreak_MessageQueue = osMessageQueueNew(1, 1, NULL);
    HomeUpdata_MessageQueue = osMessageQueueNew(1, 1, NULL);
    DataSave_MessageQueue = osMessageQueueNew(2, 1, NULL);
    HardIntEventHandle = osEventFlagsNew(NULL);

    /**
    * @brief 创建各功能任务。
    * @note HardwareInitTask完成硬件初始化后会自行删除。
    */
    HardwareInitTaskHandle  = osThreadNew(HardwareInitTask, NULL, &HardwareInitTask_attributes);
    LvHandlerTaskHandle  = osThreadNew(LvHandlerTask, NULL, &LvHandlerTask_attributes);
    WDOGFeedTaskHandle   = osThreadNew(WDOGFeedTask, NULL, &WDOGFeedTask_attributes);
    IdleEnterTaskHandle  = osThreadNew(IdleEnterTask, NULL, &IdleEnterTask_attributes);
    StopEnterTaskHandle  = osThreadNew(StopEnterTask, NULL, &StopEnterTask_attributes);
    KeyTaskHandle 			 = osThreadNew(KeyTask, NULL, &KeyTask_attributes);
    ScrRenewTaskHandle   = osThreadNew(ScrRenewTask, NULL, &ScrRenewTask_attributes);
    SensorDataTaskHandle = osThreadNew(SensorDataUpdateTask, NULL, &SensorDataTask_attributes);
    HRDataTaskHandle		 = osThreadNew(HRDataUpdateTask, NULL, &HRDataTask_attributes);
    ChargPageEnterTaskHandle = osThreadNew(ChargPageEnterTask, NULL, &ChargPageEnterTask_attributes);
    MessageSendTaskHandle = osThreadNew(MessageSendTask, NULL, &MessageSendTask_attributes);
    MPUCheckTaskHandle		= osThreadNew(MPUCheckTask, NULL, &MPUCheckTask_attributes);
    DataSaveTaskHandle		= osThreadNew(DataSaveTask, NULL, &DataSaveTask_attributes);

    /* add events, ... */


    /* add  others ... */
    uint8_t HomeUpdataStr;
    osMessageQueuePut(HomeUpdata_MessageQueue, &HomeUpdataStr, 0, 1);

}


/**
* @brief 处理FreeRTOS系统节拍派生时间。
* @details 推进LVGL时间基准，并维护计时页面、空闲检测和心率计数。
* @note 当前configTICK_RATE_HZ为1000时，1个Tick通常对应1ms。
*/
void TaskTickHook(void)
{
    //to increase the LVGL tick
    lv_tick_inc(1);
    //to increase the timerpage's timer(put in here is to ensure the Real Time)
    if(ui_TimerPageFlag)
    {
        IdleTimerCount = 0;
        ui_TimerPage_elapsed_ms += 1;
    }
    else
    {
        IdleTimerCount += 1;
    }
    user_HR_timecount+=1;
}


/**
* @brief LVGL后台任务。
* @param argument 未使用的任务参数。
* @details 周期调用lv_task_handler，处理控件事件、动画、定时器和显示刷新。
*/
void LvHandlerTask(void *argument)
{
    uint8_t IdleBreakstr=0;
    while(1)
    {
        if(lv_disp_get_inactive_time(NULL)<1000)
        {
            //Idle time break, set to 0
            osMessageQueuePut(IdleBreak_MessageQueue, &IdleBreakstr, 0, 0);
        }
        uint32_t wait = lv_task_handler();
        if(wait < 1U) wait = 1U;
        if(wait > 30U) wait = 30U;
        osDelay(wait);
    }
}


/**
* @brief 外部看门狗喂狗任务。
* @param argument 未使用的任务参数。
* @details 周期喂狗并保持看门狗使能，防止系统异常时自动复位。
*/
void WDOGFeedTask(void *argument)
{
    //owdg
    WDOG_Port_Init();
    while(1)
    {
        WDOG_Feed();
        WDOG_Enable();
        osDelay(100);
    }
}


