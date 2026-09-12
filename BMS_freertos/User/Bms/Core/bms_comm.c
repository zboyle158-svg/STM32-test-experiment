/** @file bms_comm.c
 *  @brief BMS 通信业务任务实现。
 *  @details 负责将 BMS 状态发送到外部设备或处理上位机命令；底层收发应通过 User/Drivers 接口完成。
 *  @par 使用方法 在系统初始化阶段调用 BMS_CommInit()；当前是否启用由 bms_app.c 决定。
 */
#define BMS_DBG_TAG "Comm"

#include <stdio.h>
#include <rtthread.h>

#include "main.h"

#include "bms_comm.h"

#include "bms_hal_monitor.h"
#include "bms_hal_control.h"

#include "bms_monitor.h"
#include "bms_energy.h"
#include "bms_analysis.h"
#include "bms_global.h"
#include "bms_debug.h"




// thread config
#define COMM_TASK_STACK_SIZE	512	
#define COMM_TASK_PRIORITY		13
#define COMM_TASK_TIMESLICE		25

#define COMM_TASK_PERIOD		2000






static void BMS_CommTaskEntry(void *paramter);


void BMS_CommInit(void)
{
	rt_thread_t thread;


	thread = rt_thread_create("Comm", 
							   BMS_CommTaskEntry, 
							   NULL,
							   COMM_TASK_STACK_SIZE,
							   COMM_TASK_PRIORITY,
							   COMM_TASK_TIMESLICE);

	if (thread == NULL)
	{
		BMS_ERROR("Create Task Fail");
	}

	rt_thread_startup(thread);

}


/* 目前未支持，待后续支持 */
static void BMS_CommTaskEntry(void *paramter)
{
	while(1)
	{	
		osDelay(COMM_TASK_PERIOD);
	}
}



