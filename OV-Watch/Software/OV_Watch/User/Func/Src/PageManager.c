/**
* @file PageManager.c
* @brief LVGL页面栈管理器。
* @details
* 页面对象使用后进先出栈管理。进入新页面时先退出当前页面，
* 返回时弹出当前页面并重新初始化上一个页面。
*/

#include "../Inc/PageManager.h"
#include "../../GUI_App/Screens/Inc/ui_HomePage.h"
#include "../../GUI_App/Screens/Inc/ui_MenuPage.h"

/** @brief 全局页面栈。 */
PageStack_t PageStack;

/**
* @brief 初始化页面栈。
* @param stack 要初始化的页面栈。
*/
static void page_stack_init(PageStack_t* stack)
{
    stack->top = 0;
}

/**
* @brief 将页面压入页面栈。
* @param stack 页面栈。
* @param page 要压入的页面对象。
* @return 0表示成功，非0表示页面栈已满。
*/
static uint8_t page_stack_push(PageStack_t* stack, Page_t* page)
{
    if (stack->top >= MAX_DEPTH)
    return -1;

    stack->pages[stack->top++] = page;
    return 0;
}

/**
* @brief 弹出页面栈顶页面并执行退出函数。
* @param stack 页面栈。
* @return 0表示成功，非0表示页面栈为空。
*/
static uint8_t page_stack_pop(PageStack_t* stack)
{
    if (stack->top <= 0)
    return -1;

    stack->pages[--stack->top]->deinit();
    return 0;
}

/**
* @brief 判断页面栈是否为空。
* @param stack 页面栈。
* @return 1表示为空，0表示非空。
*/
static uint8_t page_stack_is_empty(const PageStack_t* stack)
{
    return stack->top == 0;
}

/**
* @brief 获取页面栈顶页面。
* @param stack 页面栈。
* @return 栈顶页面指针；栈为空时返回NULL。
*/
static Page_t* get_top_page(PageStack_t* stack)
{
    if (stack->top == 0)
    return NULL;

    return stack->pages[stack->top - 1];
}

/**
* @brief 获取当前页面。
* @return 当前页面对象；页面栈为空时返回NULL。
*/
Page_t* Page_Get_NowPage(void)
{
    return get_top_page(&PageStack);
}

/**
* @brief 返回上一个页面。
* @details
* 弹出当前页面，初始化上一个页面并执行LVGL切换动画。
* 页面栈为空时恢复Home和Menu页面作为基础页面。
*/
void Page_Back(void)
{
    if (page_stack_is_empty(&PageStack))
    return;

    page_stack_pop(&PageStack);

    if (page_stack_is_empty(&PageStack))
    {
        page_stack_push(&PageStack, &Page_Home);
        page_stack_push(&PageStack, &Page_Menu);
        Page_Menu.init();
        lv_scr_load_anim(*Page_Menu.page_obj, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
    }
    else
    {
        Page_t *previous_page = PageStack.pages[PageStack.top - 1];
        previous_page->init();
        lv_scr_load_anim(*previous_page->page_obj, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
    }
}

/**
* @brief 返回页面栈底页面。
* @details 清理中间页面，只保留页面栈底对象并重新加载该页面。
*/
void Page_Back_Bottom(void)
{
    if (page_stack_is_empty(&PageStack))
    return;

    while (PageStack.top > 1)
    page_stack_pop(&PageStack);

    PageStack.pages[PageStack.top - 1]->init();
    lv_scr_load_anim(*PageStack.pages[PageStack.top - 1]->page_obj,
    LV_SCR_LOAD_ANIM_MOVE_RIGHT,
    100,
    0,
    true);
}

/**
* @brief 加载新页面并压入页面栈。
* @param newPage 要加载的页面对象。
* @details 当前页面存在时先调用deinit，再初始化新页面并执行切换动画。
*/
void Page_Load(Page_t *newPage)
{
    if (PageStack.top >= MAX_DEPTH - 1)
    return;

    if (PageStack.top > 0)
    PageStack.pages[PageStack.top - 1]->deinit();

    page_stack_push(&PageStack, newPage);
    newPage->init();
    lv_scr_load_anim(*newPage->page_obj, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
}

/**
* @brief 初始化页面管理器并显示主页。
* @details 创建页面栈、压入主页、初始化主页控件并加载首个LVGL屏幕。
*/
void Pages_init(void)
{
    page_stack_init(&PageStack);
    page_stack_push(&PageStack, &Page_Home);
    Page_Home.init();
    lv_disp_load_scr(*Page_Home.page_obj);
}
