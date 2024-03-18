#ifndef __LED_H
#define __LED_H

#include "stm32f10x.h"
#include "sys.h"
#include "main.h"

#define RGB_R_GPIO_PORT GPIOA
#define RGB_R_GPIO_PIN  GPIO_Pin_1
#define RGB_B_GPIO_PORT GPIOB
#define RGB_B_GPIO_PIN  GPIO_Pin_10
#define RGB_G_GPIO_PORT GPIOB
#define RGB_G_GPIO_PIN  GPIO_Pin_11
#define LED3_GPIO_PORT  GPIOA
#define LED3_GPIO_PIN   GPIO_Pin_5

#define RGB_R           PAout(1)
#define RGB_B           PBout(10)
#define RGB_G           PBout(11)
#define LED3            PAout(5)

extern void LED_GPIO_Init(void);

#endif
