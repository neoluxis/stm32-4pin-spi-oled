#include "led.h"

void LED_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = RGB_R_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(RGB_R_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = RGB_B_GPIO_PIN;
    GPIO_Init(RGB_B_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = RGB_G_GPIO_PIN;
    GPIO_Init(RGB_G_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = LED3_GPIO_PIN;
    GPIO_Init(LED3_GPIO_PORT, &GPIO_InitStructure);

    GPIO_SetBits(RGB_R_GPIO_PORT, RGB_R_GPIO_PIN);
    GPIO_SetBits(RGB_B_GPIO_PORT, RGB_B_GPIO_PIN);
    GPIO_SetBits(RGB_G_GPIO_PORT, RGB_G_GPIO_PIN);
    GPIO_SetBits(LED3_GPIO_PORT, LED3_GPIO_PIN);
}
