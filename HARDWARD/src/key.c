#include "key.h"
#include "main.h"

void KEY_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin   = KEY0_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
    GPIO_Init(KEY0_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin  = KEY2_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN;
    GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);
}

uint8_t Key_Read(uint8_t mode)
{
    static uint8_t key_up = 1;
    if (mode) {
        key_up = 1;
    }
    if (key_up && (KEY0 == 0 || KEY1 == 0 || KEY2 == 1 || KEY3 == 1)) {
        key_up = 0;
        if (KEY0 == 0) {
            return 1;
        } else if (KEY1 == 0) {
            return 2;
        } else if (KEY2 == 1) {
            return 3;
        } else if (KEY3 == 1) {
            return 4;
        }
    } else if (KEY0 == 1 && KEY1 == 1 && KEY2 == 0 && KEY3 == 0) {
        key_up = 1;
    }
    return 0;
}
