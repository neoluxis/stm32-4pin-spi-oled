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
  OLED_ShowString("Hallo! Mein Name ist Jaques. Ich komme aus Franckreich. Ich bin 25 Jahre alt. Ich bin ein Student. Ich lerne Deutsch. Ich bin ein Anfanger. Ich mochte Deutsch lernen. Ich mochte in Deutschland arbeiten. Ich mochte in Deutschland leben. Ich mochte in Deutschland studieren. Ich mochte in Deutschland reisen", 0, 0, 1, 1);
  while (1) {
  }
}
