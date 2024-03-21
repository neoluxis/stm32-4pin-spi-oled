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
  // OLED_ShowString(" Frere Jacques\r\n Frere Jacques\r\n Dormez-vous?\r\n Dormez-vous?\r\n Sonnez les ma-tines\r\n Sonnez les ma-tines\r\n Ding dang dong\r\n Ding dang dong",
  //                 0, 0, 0, 1);
  // OLED_ShowChar('V', 0, 0, 1, 1);
  // OLED_ShowChar('V', 4, 0, 1, 0);
  OLED_ShowChinese(0, 0, 0);
  OLED_ShowChinese(1, 16, 0);
  while (1) {
  }
}
