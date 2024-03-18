#ifndef __OLED_H
#define __OLED_H

#include "stm32f10x.h"
#include "sys.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

extern void OLED_Init(void);
extern void OLED_Clear(void);
extern void OLED_ShowChar(uint8_t Line, uint8_t Column, char Char);
extern void OLED_ShowString(uint8_t Line, uint8_t Column, char *String);
extern void OLED_ShowNum(uint8_t Line, uint8_t Column, uint32_t Number, uint8_t Length);
extern void OLED_ShowChinese(uint8_t Line, uint8_t Column, uint8_t No);

#endif
