/**
 * @file    oled.h
 * @brief   OLED驱动头文件
 * @details 12864驱动, 使用四角SPI接口, 主控芯片SSD1315
 * @author Neolux Lee
 * @date 2024-03-20
 */

#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"
#include "sys.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "math.h"
#include "oled/bmp.h"
#include "oled/font.h"

/**
 * @brief  OLED显示缓存
 * @note   8行128列 - 1个字节表示8个像素
 */
extern uint8_t OLED_GRAM[8][128];

/**
 * 默认字体设置和字体宽高
 */
#define DEFAULT_ASCII_FONT FONT_ASCII_8x16
#define ASCII_CW           8
#define ASCII_CH           2

#define DEFAULT_CN_FONT    FONT_CN_16x16
#define CN_CW              16
#define CN_CH              2

/**
 * @brief  OLED初始化
 * @param  None
 * @retval None
 */
extern void OLED_Init(void);

/**
 * @brief  清除OLED屏幕
 * @param  inverse: 是否反色 0:正常 1:反色
 * @retval None
 */
extern void OLED_Clear(uint8_t inverse);

/**
 * @brief  OLED写命令
 * @param  Command: 要写入的命令
 * @retval None
 */
extern void OLED_WriteCmd(uint8_t Command);

/**
 * @brief  OLED写数据
 * @param  Data: 要写入的数据
 * @retval None
 */
extern void OLED_WriteData(uint8_t Data);

/**
 * @brief  设置OLED显示位置
 * @param  x: x坐标
 * @param  y: y坐标
 * @retval None
 */
extern void OLED_SetPos(uint8_t x, uint8_t y);

/**
 * @brief  将GRAM中的数据刷到OLED屏幕
 * @param  None
 * @retval None
 */
extern void OLED_PushGRAM(void);

/**
 * @brief  在OLED缓存中显示一个点，不刷新到屏幕，便于加快其他图形绘制
 * @param  x: x坐标, 0-127
 * @param  y: y坐标, 0-63
 * @param  inverse: 是否反色 0:正常 1:反色
 */
extern void OLED_Dot(uint8_t x, uint8_t y, uint8_t inverse);

/**
 * @brief  在OLED上显示一个点
 * @param  x: x坐标, 0-127
 * @param  y: y坐标, 0-63
 * @param  inverse: 是否反色 0:正常 1:反色
 */
extern void OLED_DrawDot(uint8_t x, uint8_t y, uint8_t inverse);

/**
 * @brief  在OLED上显示一个线
 * @param  x1: 起始x坐标, 0-127
 * @param  y1: 起始y坐标, 0-63
 * @param  x2: 终止x坐标, 0-127
 * @param  y2: 终止y坐标, 0-63
 * @param  inverse: 是否反色 0:正常 1:反色
 */
extern void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                          uint8_t inverse);

/**
 * @brief  在OLED上显示一个矩形
 * @param x: 起始x坐标, 0-127
 * @param y: 起始y坐标, 0-63
 * @param w: 矩形宽度
 * @param h: 矩形高度
 * @param full: 是否填充 0:不填充 1:填充
 * @param inverse: 是否反色 0:正常 1:反色
 */
extern void OLED_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h,
                          uint8_t full, uint8_t inverse);

/**
 * @brief  在OLED上显示一个圆
 * @param  x: 圆心x坐标, 0-127
 * @param  y: 圆心y坐标, 0-63
 * @param  r: 圆半径
 * @param  full: 是否填充 0:不填充 1:填充
 * @param  inverse: 是否反色 0:正常 1:反色
 */
extern void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r,
                            uint8_t full, uint8_t inverse);

/**
 * @brief  在OLED上显示ASCII字符
 * @param  C: 要显示的字符
 * @param  x: x坐标, 0-127
 * @param  y: y坐标, 0-63
 */
extern void OLED_ShowChar(uint8_t C, uint8_t x, uint8_t y);

/**
 * @brief  在OLED上显示字符串
 * @param  string: 要显示的字符串
 * @param  x: x坐标, 0-127
 * @param  y: y坐标, 0-63
 */
extern void OLED_ShowString(char *string, uint8_t x, uint8_t y);

/**
 * @brief  在OLED上显示数字
 * @param  num: 要显示的数字
 * @param  x: x坐标, 0-127
 * @param  y: y坐标, 0-63
 */
extern void OLED_ShowNumber(int32_t num, uint8_t x, uint8_t y);

/**
 * @brief  在OLED上显示两色位图, 图片高度必须是8的倍数
 * @param  bmp: 位图数据
 * @param  x: x坐标, 0-127
 * @param  y: y坐标, 0-63
 * @param  w: 位图宽度
 * @param  h: 位图高度
 */
extern void OLED_ShowBMP(const uint8_t bmp[], uint8_t x, uint8_t y, uint8_t w, uint8_t h);

/**
 * @brief  在OLED上显示中文字符
 * @param  Cidx: 要显示的字符在字库中的位置
 * @param  x: x坐标, 0-127
 * @param  y: y坐标, 0-63
 */
extern void OLED_ShowChinese(uint8_t Cidx, uint8_t x, uint8_t y);

#endif
