#include "stm32f10x.h"
#include "sys.h"
#include "main.h"
#include "led.h"
#include "oled/oled.h"
#include "delay.h"
#include "oled/bmp.h"

int main(int argc, char const *argv[])
{
  LED_GPIO_Init();
  delay_init();
  OLED_Init();
  // OLED_ShowString("Frere Jacques\r\nFrere Jacques\r\nDormez-vous?\r\nDormez-vous?\r\nSonnez les matines\r\nSonnez les matines\r\nDing ding dong\r\nDing ding dong",
  //                 0, 0);
  OLED_ShowBMP(bmp1, 0, 0, 16, 16);
  while (1) {
  }
}
