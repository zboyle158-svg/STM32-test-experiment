/** @file bms_global.h
 *  @brief BMS 系统模式和充放电许可的共享状态。
 *  @details 通过 extern 导出唯一全局对象 BMS_GlobalParam；定义位于 bms_global.c。
 *  @par 使用方法 业务任务读取状态，只有保护和能量管理模块应修改许可字段。
 */
#ifndef __BMS_GLOBAL_H__
#define __BMS_GLOBAL_H__


#include "bms_type.h"


extern BMS_GlobalParamTypedef BMS_GlobalParam;


#endif

