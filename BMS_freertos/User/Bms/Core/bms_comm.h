/** @file bms_comm.h
 *  @brief BMS 通信任务公共接口。
 *  @details 只暴露任务创建函数，通信协议和帧处理隐藏在实现文件中。
 *  @par 使用方法 RTOS 内核启动后调用 BMS_CommInit() 一次。
 */
#ifndef __BMS_COMM_H__
#define __BMS_COMM_H__

void BMS_CommInit(void);


#endif



