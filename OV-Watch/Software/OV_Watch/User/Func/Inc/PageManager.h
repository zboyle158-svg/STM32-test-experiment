/**
* @file PageManager.h
* @brief OV-Watch应用功能中间层实现。
* @details
* 本文件仅实现本模块职责；底层硬件访问通过对应HAL/BSP接口完成。
* 修改本文件时应保持公开接口、返回值约定、时序和硬件引脚配置不变。
*/

#ifndef PAGE_STACK_H
#define PAGE_STACK_H

#include "../../GUI_App/ui.h"

// 页面栈深度
#define MAX_DEPTH 6

// 页面结构体
typedef struct {
    void (*init)(void);
    void (*deinit)(void);
    lv_obj_t **page_obj;
} Page_t;


// 页面堆栈结构体
typedef struct {
    Page_t* pages[MAX_DEPTH];
    uint8_t top;
} PageStack_t;

extern PageStack_t PageStack;


Page_t* Page_Get_NowPage(void);
void Page_Back(void);
void Page_Back_Bottom(void);
void Page_Load(Page_t *newPage);
void Pages_init(void);


#endif // PAGE_STACK_H
