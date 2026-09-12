/** @file bms_debug.h
 *  @brief BMS 统一日志宏。
 *  @details 将调试、信息和错误输出统一附加模块标签、文件、函数及行号。
 *  @par 使用方法 在 .c 文件中定义 BMS_DBG_TAG 后包含本文件，并使用 BMS_INFO/BMS_ERROR 等宏。
 */
#ifndef __BMS_DEBUG_H__
#define __BMS_DEBUG_H__


#include <rtthread.h>
#include "usart.h"


#define BMS_DEBUG_LEVEL	1


#ifdef BMS_DBG_TAG

#ifndef BMS_DBG_SECTION_NAME
#define BMS_DBG_SECTION_NAME	BMS_DBG_TAG
#endif

#else

#ifndef BMS_DBG_SECTION_NAME
#define BMS_DBG_SECTION_NAME    "DBG"
#endif
#endif 



#if (BMS_DEBUG_LEVEL == 0)

#define BMS_ERROR(...)		do{}while(0)
#define BMS_WARNING(...)	do{}while(0)
#define BMS_INFO(...)		do{}while(0)

#elif (BMS_DEBUG_LEVEL == 1)

#define BMS_ERROR(fmt, arg...)                                		\
		do															\
        {                                                       	\
            print_usart1("[BMS ERROR][%s]", BMS_DBG_SECTION_NAME);    \
            print_usart1(fmt"\r\n", ##arg);                     		\
        } while(0)
#define BMS_WARNING(fmt, arg...)                             		\
		do															\
        {                                                       	\
            print_usart1("[BMS WARNING][%s]", BMS_DBG_SECTION_NAME);  \
            print_usart1(fmt"\r\n", ##arg);                      		\
        } while(0)
#define BMS_INFO(fmt, arg...)                                  		\
		do															\
        {                                                       	\
            print_usart1("[BMS INFO][%s]", BMS_DBG_SECTION_NAME);     \
            print_usart1(fmt"\r\n", ##arg);                      		\
        } while(0)


#elif (BMS_DEBUG_LEVEL == 2)

#define BMS_ERROR(fmt, arg...)                              					\
		do																		\
        {                                                      					\
            print_usart1("[BMS ERROR][%s][%s:%s:%d] ",							\
                    BMS_DBG_SECTION_NAME, __FILE__, __FUNCTION__, __LINE__);    \
            print_usart1(fmt"\r\n", ##arg);                      					\
        } while(0)
#define BMS_WARNING(fmt, arg...)                               					\
		do																		\
        {                                                       				\
            print_usart1("[BMS WARNING][%s][%s:%s:%d] ",							\
                    BMS_DBG_SECTION_NAME, __FILE__, __FUNCTION__, __LINE__);    \
            print_usart1(fmt"\r\n", ##arg);                      					\
        } while(0)
#define BMS_INFO(fmt, arg...)                                  					\
		do																		\
        {                                                       				\
            print_usart1("[BMS INFO][%s][%s:%s:%d] ",								\
                    BMS_DBG_SECTION_NAME, __FILE__, __FUNCTION__, __LINE__);    \
            print_usart1(fmt"\r\n", ##arg);										\
        } while(0)
        
#endif

		
		
		
		
#endif
		
		
