#include "stm32f10x.h"
#include "sys.h"
#include "main.h"
#include "led.h"
#include "oled/oled.h"
#include "delay.h"

int main(int argc, char const *argv[])
{
    LED_GPIO_Init();
    delay_init();
    OLED_Init();
    OLED_ShowChar('H', 8, 0, 0, 1);
    OLED_ShowChar('H', 0, 5, 0, 1);
    OLED_ShowChar('H', 0, 5 + 16, 0, 1);
    while (1) {
    }
}
