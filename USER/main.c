#include "stm32f10x.h"
#include "sys.h"
#include "main.h"
#include "led.h"
#include "oled/oled.h"

int main(int argc, char const *argv[])
{
    LED_GPIO_Init();
    OLED_Init();
    while (1) {
        OLED_ShowChar('H', 0, 0, 0, 1);
    }
}
