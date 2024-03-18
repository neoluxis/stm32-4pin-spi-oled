#ifndef __KEY_H
#define __KEY_H

#include "main.h"

#define KEY0_GPIO_PORT GPIOA
#define KEY0_GPIO_PIN  GPIO_Pin_6
#define KEY1_GPIO_PORT GPIOA
#define KEY1_GPIO_PIN  GPIO_Pin_7
#define KEY2_GPIO_PORT GPIOB
#define KEY2_GPIO_PIN  GPIO_Pin_0
#define KEY3_GPIO_PORT GPIOB
#define KEY3_GPIO_PIN  GPIO_Pin_1

#define KEY0           GPIO_ReadInputDataBit(KEY0_GPIO_PORT, KEY0_GPIO_PIN)
#define KEY1           GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN)
#define KEY2           GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_GPIO_PIN)
#define KEY3           GPIO_ReadInputDataBit(KEY3_GPIO_PORT, KEY3_GPIO_PIN)

extern void KEY_GPIO_Init(void);
extern uint8_t Key_Read(uint8_t mode);

#endif
