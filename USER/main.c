#include "stm32f10x.h"
#include "sys.h"
#include "main.h"
#include "led.h"
#include "oled/oled.h"
#include "delay.h"
#include "stdio.h"

int main(int argc, char const *argv[])
{
    LED_GPIO_Init();
    delay_init();
    OLED_Init();
    OLED_ShowChar('H', 8, 13, 0, 1);
    OLED_ShowChar('H', 8, 45, 0, 1);
    char str[100];
    sprintf(str, "%02x", OLED_GRAM[4][8]);
    OLED_ShowString(str,50, 0, 0,1);
    OLED_ShowChar('H', 8, 29, 1, 1);
    sprintf(str, "%02x", OLED_GRAM[4][8]);
    OLED_ShowString(str,50, 16, 0,1);
    OLED_ShowNumber(1234567890, 0, 0, 0, 1);
    while (1) {
    }
}
