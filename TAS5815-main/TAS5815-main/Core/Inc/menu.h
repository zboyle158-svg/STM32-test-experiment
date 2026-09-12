/* USER CODE BEGIN Header */
/** @details 编码器菜单状态机接口。Menu_Process在主循环轮询，短按确认，长按返回；菜单动作通过驱动接口修改硬件并保存EEPROM。 */
/**
  ******************************************************************************
  * @file    menu.h
  * @brief   多级菜单系统头文件
  ******************************************************************************
  * @attention
  * 多级菜单系统，支持编码器选择和按键确认
  * 开机默认为音量调节界面，长按按键进入菜单
  ******************************************************************************
  */
/* USER CODE END Header */

#ifndef __MENU_H
#define __MENU_H

#include "main.h"
#include <stdint.h>

#define MAIN_MENU_SIZE  7  /* 主菜单项数量 */

/* 菜单状态枚举 */
/**
 * @brief 菜单状态。
 * @note MENU_STATE_VOLUME是默认工作页面，其音量处理由main.c完成；其余状态由menu.c绘制和处理。
 */
typedef enum {
    MENU_STATE_VOLUME = 0,    /* 音量调节界面（默认） */
    MENU_STATE_MAIN,          /* 一级菜单 */
    MENU_STATE_BLUETOOTH,     /* 蓝牙设置子菜单 */
    MENU_STATE_VOLTAGE,       /* 诱骗电压子菜单 */
    MENU_STATE_AMP,           /* AMP设置子菜单 */
    MENU_STATE_MODULATION,    /* 调制模式子菜单 */
    MENU_STATE_FSW,           /* 开关频率子菜单 */
    MENU_STATE_POWEROFF       /* 关机设置子菜单 */
} MenuState;

/* 菜单项结构体 */
/**
 * @brief 一个菜单项目的静态描述。
 * @details action是短按后执行的函数指针；subMenu和subMenuSize描述可进入的子菜单。
 */
typedef struct MenuItem {
    const char *name;                    /* 菜单项名称 */
    MenuState targetState;               /* 目标状态 */
    void (*action)(void);                /* 执行函数（可选） */
    struct MenuItem *subMenu;            /* 子菜单指针 */
    uint8_t subMenuSize;                 /* 子菜单大小 */
} MenuItem;

/* 菜单系统结构体 */
/**
 * @brief 菜单运行时状态。
 * @note currentMenu指向静态菜单数组，不指向动态分配内存；selectedIndex是当前选中下标。
 */
typedef struct {
    MenuState currentState;              /* 当前菜单状态 */
    MenuItem *currentMenu;               /* 当前菜单项数组 */
    uint8_t currentMenuSize;             /* 当前菜单项数量 */
    uint8_t selectedIndex;               /* 当前选中索引 */
    int32_t encoderCount;                /* 编码器计数缓存 */
    uint8_t inSubMenu;                   /* 是否在子菜单中 */
} MenuSystem;

/* 菜单系统初始化 */
/** @brief 初始化菜单状态并默认进入音量页面。 */
void Menu_Init(void);

/* 菜单主处理函数 */
/**
 * @brief 在主循环中轮询编码器和按键，并更新菜单界面。
 * @note 函数可能执行OLED I2C刷新和短时按键动作，不应从中断服务函数调用。
 */
void Menu_Process(void);

/* 获取当前菜单状态 */
/** @brief 返回当前菜单状态。 */
MenuState Menu_GetState(void);

/* 进入菜单 */
/** @brief 从音量页面进入一级菜单。 */
void Menu_Enter(void);

/* 退出菜单返回音量界面 */
/** @brief 退出菜单并返回音量页面，同时同步TIM2计数器。 */
void Menu_Exit(void);

/* 绘制当前菜单 */
/** @brief 按当前状态和选中项目绘制一帧菜单画面。 */
void Menu_Draw(void);

/* 编码器处理 */
/** @brief 根据编码器增量移动选中项目；正值向下，负值向上。 */
void Menu_EncoderHandler(int32_t encoderDelta);

/* 按键处理 */
/** @brief 执行一次短按确认动作；长按逻辑在Menu_Process中完成。 */
void Menu_ButtonHandler(void);

/* 关机状态查询 */
/** @brief 查询是否产生了待执行的关机请求。 */
uint8_t Menu_IsPowerOffPending(void);
/** @brief 清除已经被main.c取走的关机请求。 */
void Menu_ClearPowerOffPending(void);
/** @brief 查询当前是否正在显示长按关机进度。 */
uint8_t Menu_IsInPowerOffMode(void);

#endif /* __MENU_H */
