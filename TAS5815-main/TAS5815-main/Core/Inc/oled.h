#ifndef __OLED_H__
#define __OLED_H__

#include "font.h"
#include "main.h"
#include "string.h"

/** @brief OLED像素绘制颜色；底层显存采用按位表示的单色格式。 */
typedef enum {
  OLED_COLOR_NORMAL = 0, // 正常模式 黑底白字
  OLED_COLOR_REVERSED    // 反色模式 白底黑字
} OLED_ColorMode;

/** @brief 发送SSD1306初始化命令并清空屏幕。 */
void OLED_Init();
/** @brief 打开OLED电荷泵和显示输出。 */
void OLED_DisPlay_On();
/** @brief 关闭OLED电荷泵和显示输出以降低功耗。 */
void OLED_DisPlay_Off();

/** @brief 清空RAM中的整帧显存，后续绘图从空白画布开始。 */
void OLED_NewFrame();
/** @brief 将RAM显存按页通过I2C3刷新到SSD1306。 */
void OLED_ShowFrame();
/** @brief 修改一个像素，坐标范围为x=0~127、y=0~63。 */
void OLED_SetPixel(uint8_t x, uint8_t y, OLED_ColorMode color);

void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, OLED_ColorMode color);
void OLED_DrawRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, OLED_ColorMode color);
void OLED_DrawFilledRectangle(uint8_t x, uint8_t y, uint8_t w, uint8_t h, OLED_ColorMode color);
void OLED_DrawTriangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, OLED_ColorMode color);
void OLED_DrawFilledTriangle(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t x3, uint8_t y3, OLED_ColorMode color);
void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r, OLED_ColorMode color);
void OLED_DrawFilledCircle(uint8_t x, uint8_t y, uint8_t r, OLED_ColorMode color);
void OLED_DrawEllipse(uint8_t x, uint8_t y, uint8_t a, uint8_t b, OLED_ColorMode color);
void OLED_DrawImage(uint8_t x, uint8_t y, const Image *img, OLED_ColorMode color);

/** @brief 使用指定ASCII字模绘制一个字符。 */
void OLED_PrintASCIIChar(uint8_t x, uint8_t y, char ch, const ASCIIFont *font, OLED_ColorMode color);
/** @brief 使用指定ASCII字模绘制以\0结尾的字符串。 */
void OLED_PrintASCIIString(uint8_t x, uint8_t y, char *str, const ASCIIFont *font, OLED_ColorMode color);
/** @brief 按UTF-8字节长度查找字模，并对ASCII字符使用默认字模。 */
void OLED_PrintString(uint8_t x, uint8_t y, char *str, const Font *font, OLED_ColorMode color);

#endif // __OLED_H__
