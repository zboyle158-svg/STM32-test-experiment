/**
 * @file font.h
 * @brief OLED字体、图像和字模元数据声明。
 * @details font.c提供只读点阵数组；oled.c根据本文件中的宽高和数据指针绘制ASCII及UTF-8字符。
 * @par 使用方法 直接将Font/ASCIIFont对象地址传给OLED_PrintString或OLED_PrintASCIIString。
 */
#ifndef __FONT_H
#define __FONT_H
#include "stdint.h"
#include "string.h"
/**
 * @brief ASCII字模表描述。
 * @details chars指向连续字模数据，每个字符的位图布局由h和w决定。
 */
typedef struct ASCIIFont {
  uint8_t h;
  uint8_t w;
  uint8_t *chars;
} ASCIIFont;

extern const ASCIIFont afont8x6;
extern const ASCIIFont afont12x6;
extern const ASCIIFont afont16x8;
extern const ASCIIFont afont24x12;

/**
 * @brief 字体结构体
 * @note  字库前4字节存储utf8编码 剩余字节存储字模数据
 * @note 字库数据可以使用波特律动LED取模助手生成(https://led.baud-dance.com)
 */
/**
 * @brief UTF-8字符字模表描述。
 * @details 每个字模记录前面保存UTF-8编码，后面保存位图；ascii用于找不到字符时的回退显示。
 */
typedef struct Font {
  uint8_t h;              // 字高度
  uint8_t w;              // 字宽度
  const uint8_t *chars;   // 字库 字库前4字节存储utf8编码 剩余字节存储字模数据
  uint8_t len;            // 字库长度 超过256则请改为uint16_t
  const ASCIIFont *ascii; // 缺省ASCII字体 当字库中没有对应字符且需要显示ASCII字符时使用
} Font;

extern const Font font16x16;
extern const Font font14x14;
extern const Font font30x30;
extern const Font fontNum30x30;
extern const Font font20x20;
extern const Font font25x25;
extern const Font font10x10;

/**
 * @brief 图片结构体
 * @note  图片数据可以使用波特律动LED取模助手生成(https://led.baud-dance.com)
 */
/**
 * @brief 单色图片描述。
 * @param w 图片宽度，单位像素。
 * @param h 图片高度，单位像素。
 * @param data 图片位图数据，只读存储在Flash中。
 */
typedef struct Image {
  uint8_t w;           // 图片宽度
  uint8_t h;           // 图片高度
  const uint8_t *data; // 图片数据
} Image;

extern const Image bilibiliImg;

#endif // __FONT_H
