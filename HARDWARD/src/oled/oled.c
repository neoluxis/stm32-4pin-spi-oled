/**
 * @file oled.c
 * @brief OLED驱动源文件
 * @details 12864驱动, 使用四角SPI接口, 主控芯片SSD1315
 * @author Neolux Lee
 * @date 2024-03-20
 */

#include "oled/oled.h"
#include "stm32f10x.h"
#include "oled/font.h"
#include "oled/bmp.h"
#include "stdlib.h"

uint8_t OLED_GRAM[8][128] = {0};

#define OLED_W_D0(x)   GPIO_WriteBit(GPIOB, GPIO_Pin_7, (BitAction)(x))
#define OLED_W_D1(x)   GPIO_WriteBit(GPIOB, GPIO_Pin_6, (BitAction)(x))
#define OLED_W_RES(x)  GPIO_WriteBit(GPIOB, GPIO_Pin_5, (BitAction)(x))
#define OLED_W_DC(x)   GPIO_WriteBit(GPIOB, GPIO_Pin_4, (BitAction)(x))
#define OLED_W_CS(x)   GPIO_WriteBit(GPIOB, GPIO_Pin_3, (BitAction)(x))

#define SHOW_ANIMATION 0
#if SHOW_ANIMATION
#define OLED_DrawPoint(x, y, inverse) OLED_DrawDot((x), (y), (inverse))
#else
#define OLED_DrawPoint(x, y, inverse) OLED_Dot((x), (y), (inverse))
#endif

void OLED_SPI_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  // 打开GPIO口时钟, 先打开复用才能修改复用功能
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
  // 要先开时钟, 再重映射; 这句表示关闭jtag, 使能swd
  GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  OLED_W_D0(1);
  OLED_W_D1(1);
  OLED_W_RES(1);
  OLED_W_DC(1);
  OLED_W_CS(1);
}

void OLED_SPI_SendByte(uint8_t Byte)
{
  uint8_t i;
  for (i = 0; i < 8; i++) {
    OLED_W_D0(0);
    OLED_W_D1(Byte & (0x80 >> i));
    OLED_W_D0(1);
  }
}

void OLED_WriteCmd(uint8_t Command)
{
  OLED_W_CS(0);
  OLED_W_DC(0);
  OLED_SPI_SendByte(Command);
  OLED_W_CS(1);
}

void OLED_WriteData(uint8_t Data)
{
  OLED_W_CS(0);
  OLED_W_DC(1);
  OLED_SPI_SendByte(Data);
  OLED_W_CS(1);
}

void OLED_SetPos(uint8_t x, uint8_t y)
{
  OLED_WriteCmd(0xB0 | y);                 // 设置Y位置
  OLED_WriteCmd(0x10 | ((x & 0xF0) >> 4)); // 设置X位置高4位
  OLED_WriteCmd(0x00 | (x & 0x0F));        // 设置X位置低4位
}

void OLED_PushGRAM()
{
  uint8_t x, y;
  for (y = 0; y < 8; y++) {
    for (x = 0; x < 128; x++) {
      OLED_SetPos(x, y);
      OLED_WriteData(OLED_GRAM[y][x]);
    }
  }
}

void OLED_Clear(uint8_t inverse)
{
  uint8_t x, y;
  for (y = 0; y < 8; y++) {
    for (x = 0; x < 128; x++) {
      if (inverse) {
        OLED_GRAM[y][x] = 0xFF;
      } else {
        OLED_GRAM[y][x] = 0x00;
      }
    }
  }
  OLED_PushGRAM();
}

void OLED_Dot(uint8_t x, uint8_t y, uint8_t inverse)
{
  if (x > 127 || y > 63) {
    return;
  }
  if (inverse) {
    OLED_GRAM[y / 8][x] &= ~(1 << (y % 8));
  } else {
    OLED_GRAM[y / 8][x] |= (1 << (y % 8));
  }
}

void OLED_DrawDot(uint8_t x, uint8_t y, uint8_t inverse)
{
  OLED_Dot(x, y, inverse);
  OLED_PushGRAM();
}

void OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2,
                   uint8_t inverse)
{
  int16_t dx, dy, tx, ty, err;
  dx = x2 - x1;
  dy = y2 - y1;
  tx = (dx > 0) ? 1 : -1;
  ty = (dy > 0) ? 1 : -1;
  dx = (dx > 0) ? dx : -dx;
  dy = (dy > 0) ? dy : -dy;

  if (dx > dy) {
    err = dx / 2;
    while (x1 != x2) {
      OLED_DrawPoint(x1, y1, inverse);
      err -= dy;
      if (err < 0) {
        y1 += ty;
        err += dx;
      }
      x1 += tx;
    }
  } else {
    err = dy / 2;
    while (y1 != y2) {
      OLED_DrawPoint(x1, y1, inverse);
      err -= dx;
      if (err < 0) {
        x1 += tx;
        err += dy;
      }
      y1 += ty;
    }
  }
  OLED_DrawPoint(x1, y1, inverse);
  OLED_PushGRAM();
}

void OLED_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h,
                   uint8_t full, uint8_t inverse)
{
  uint8_t i, j;
  if (full) {
    for (i = 0; i < h; i++) {
      for (j = 0; j < w; j++) {
        OLED_DrawPoint(x + j, y + i, inverse);
      }
    }
  } else {
    for (i = 0; i < w; i++) {
      OLED_DrawPoint(x + i, y, inverse);
      OLED_DrawPoint(x + i, y + h - 1, inverse);
    }
    for (i = 0; i < h; i++) {
      OLED_DrawPoint(x, y + i, inverse);
      OLED_DrawPoint(x + w - 1, y + i, inverse);
    }
  }
  OLED_PushGRAM();
}

void OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r,
                     uint8_t full, uint8_t inverse)
{
  int16_t a, b, num;
  a = 0;
  b = r;
  while (a <= b) {
    if (full) {
      for (num = y - b; num <= y + b; num++) {
        OLED_DrawPoint(x + a, num, inverse);
        OLED_DrawPoint(x - a, num, inverse);
      }
      for (num = x - b; num <= x + b; num++) {
        OLED_DrawPoint(num, y + a, inverse);
        OLED_DrawPoint(num, y - a, inverse);
      }
    } else {
      OLED_DrawPoint(x + a, y + b, inverse);
      OLED_DrawPoint(x - a, y + b, inverse);
      OLED_DrawPoint(x + a, y - b, inverse);
      OLED_DrawPoint(x - a, y - b, inverse);
      OLED_DrawPoint(x + b, y + a, inverse);
      OLED_DrawPoint(x - b, y + a, inverse);
      OLED_DrawPoint(x + b, y - a, inverse);
      OLED_DrawPoint(x - b, y - a, inverse);
    }
    a++;
    if ((a * a + b * b) > (r * r)) {
      b--;
    }
  }
  OLED_PushGRAM();
}

void OLED_ShowChar(uint8_t C, uint8_t x, uint8_t y)
{
  uint8_t i, j;
  if (x > 127 - ASCII_CW || y > 63) {
    return;
  }
  // 截取字体窗口
  uint8_t font_window[ASCII_CH + 1][ASCII_CW] = {0};
  // 最上面要被刷新的行数
  uint8_t topshow = 8 - (y % 8);
  // 最下面要被刷新的行数
  uint8_t bottomshow = y % 8;

  // 先写好新的字体窗口, 然后一起更新到GRAM, 最后刷新到屏幕
  for (i = 0; i < ASCII_CW; i++) {
    // 保存最上字节原来的数据
    font_window[0][i] = OLED_GRAM[y / 8][x + i];
    // 清除最上字节的数据
    font_window[0][i] &= (0xff >> topshow);
    // 添加新的数据
    font_window[0][i] |=
        (DEFAULT_ASCII_FONT[C - ' '][i] << bottomshow);
    OLED_GRAM[y / 8][x + i] = font_window[0][i];
    for (j = 1; j < ASCII_CH; j++) {
      // 生成并保存中间字节原来的数据
      font_window[j][i] =
          ((DEFAULT_ASCII_FONT[C - ' ']
                              [i + (j - 1) * ASCII_CW] >>
            topshow) | // f8h << 5
           (DEFAULT_ASCII_FONT[C - ' ']
                              [i + j * ASCII_CW]
            << bottomshow));
      OLED_GRAM[y / 8 + j][x + i] = font_window[j][i];
    }
    // 保存最下字节原来的数据
    font_window[ASCII_CH][i] = OLED_GRAM[y / 8 + ASCII_CH][x + i];
    // 清除最下字节的数据
    font_window[ASCII_CH][i] &= (0xff << bottomshow);
    // 添加新的数据
    font_window[ASCII_CH][i] |=
        (DEFAULT_ASCII_FONT[C - ' ']
                           [i + (ASCII_CH - 1) * ASCII_CW] >>
         topshow);
    OLED_GRAM[y / 8 + ASCII_CH][x + i] = font_window[ASCII_CH][i];
  }
  OLED_PushGRAM();
}

void OLED_ShowString(char *string, uint8_t x, uint8_t y)
{
  while (*string) {
    if (*string == '\n') {
      y = y + ASCII_CH * 8;
      string++;
      continue;
    } else if (*string == '\r') {
      x = 0;
      string++;
      continue;
    }
    OLED_ShowChar(*string, x, y);
    x = x + ASCII_CW;
    if (x > 127 - ASCII_CW) {
      x = 0;
      y = y + ASCII_CH * 8;
    }
    if (y > 63) {
      OLED_Clear(0);
      y = 0;
      x = 0;
      continue;
    }
    string++;
  }
}

void OLED_ShowNumber(int32_t num, uint8_t x, uint8_t y)
{
  char str[15];
  sprintf(str, "%d", num);
  OLED_ShowString(str, x, y);
}

void OLED_ShowBMP(const uint8_t bmp[], uint8_t x, uint8_t y,
                  uint8_t w, uint8_t h)
{
  uint8_t hRemain = h % 8;
  uint8_t hByte   = h / 8 + (hRemain ? 1 : 0);
  uint8_t i, j;
  if (x > 127 - w || y > 63 - h) {
    return;
  }
  // 截取BMP窗口
  uint8_t **bmp_window = (uint8_t **)malloc((hByte + 1) * sizeof(uint8_t *));
  for (i = 0; i < hByte + 1; i++) {
    bmp_window[i] = (uint8_t *)malloc(w * sizeof(uint8_t));
    for (j = 0; j < w; j++) {
      bmp_window[i][j] = 0;
    }
  }

  // 最上面要被刷新的行数
  uint8_t topshow = 8 - (y % 8);
  // 最下面要被刷新的行数
  uint8_t bottomshow = y % 8;
  // 先写好新的BMP窗口, 然后一起更新到GRAM, 最后刷新到屏幕
  for (j = 0; j < w; j++) {
    bmp_window[0][j] = OLED_GRAM[y / 8][x + j];
    bmp_window[0][j] &= (0xff >> topshow);
    bmp_window[0][j] |= (bmp[j] << bottomshow);
    OLED_GRAM[y / 8][x + j] = bmp_window[0][j];
    for (i = 1; i < hByte; i++) {
      bmp_window[i][j]            = ((bmp[j + i * w] >> topshow) | (bmp[j + (i - 1) * w] << bottomshow));
      OLED_GRAM[y / 8 + i][x + j] = bmp_window[i][j];
    }
    bmp_window[hByte][j] = OLED_GRAM[y / 8 + hByte][x + j];
    bmp_window[hByte][j] &= (0xff << (bottomshow + hRemain));
    bmp_window[hByte][j] |= (bmp[j + (hByte - 1) * w] >> topshow);
    OLED_GRAM[y / 8 + hByte][x + j] = bmp_window[hByte][j];
  }
  OLED_PushGRAM();
}

void OLED_ShowChinese(uint8_t Cidx, uint8_t x, uint8_t y)
{
  uint8_t i, j;
  if (x > 127 - CN_CW || y > 63 - CN_CH * 8) {
    return;
  }
  // 截取字体窗口
  uint8_t font_window[CN_CH + 1][CN_CW] = {0};
  // 最上面要被刷新的行数
  uint8_t topshow = 8 - (y % 8);
  // 最下面要被刷新的行数
  uint8_t bottomshow = y % 8;

  // 先写好新的字体窗口, 然后一起更新到GRAM, 最后刷新到屏幕
  for (i = 0; i < CN_CW; i++) {
    // 保存最上字节原来的数据
    font_window[0][i] = OLED_GRAM[y / 8][x + i];
    // 清除最上字节的数据
    font_window[0][i] &= (0xff >> topshow);
    // 添加新的数据
    font_window[0][i] |=
        (DEFAULT_CN_FONT[Cidx][i] << bottomshow);
    OLED_GRAM[y / 8][x + i] = font_window[0][i];
    for (j = 1; j < CN_CH; j++) {
      // 生成并保存中间字节原来的数据
      font_window[j][i] =
          ((DEFAULT_CN_FONT[Cidx][i + (j - 1) * CN_CW] >>
            topshow) | // f8h << 5
           (DEFAULT_CN_FONT[Cidx][i + j * CN_CW]
            << bottomshow));
      OLED_GRAM[y / 8 + j][x + i] = font_window[j][i];
    }
    // 保存最下字节原来的数据
    font_window[CN_CH][i] = OLED_GRAM[y / 8 + CN_CH][x + i];
    // 清除最下字节的数据
    font_window[CN_CH][i] &= (0xff << bottomshow);
    // 添加新的数据
    font_window[CN_CH][i] |=
        (DEFAULT_CN_FONT[Cidx][i + (CN_CH - 1) * CN_CW] >>
         topshow);
    OLED_GRAM[y / 8 + CN_CH][x + i] = font_window[CN_CH][i];
  }
  OLED_PushGRAM();
}

void OLED_Init(void)
{
  OLED_SPI_Init(); // 端口初始化

  OLED_WriteCmd(0xAE); // 关闭显示

  OLED_WriteCmd(0xD5); // 设置显示时钟分频比/振荡器频率
  OLED_WriteCmd(0xF0);

  OLED_WriteCmd(0xA8); // 设置多路复用率
  OLED_WriteCmd(0x3F);

  OLED_WriteCmd(0xD3); // 设置显示偏移
  OLED_WriteCmd(0x00);

  OLED_WriteCmd(0x40); // 设置显示开始行

  OLED_WriteCmd(0xA1); // 设置左右方向，0xA1正常 0xA0左右反置

  OLED_WriteCmd(0xC8); // 设置上下方向，0xC8正常 0xC0上下反置

  OLED_WriteCmd(0xDA); // 设置COM引脚硬件配置
  OLED_WriteCmd(0x12);

  OLED_WriteCmd(0x81); // 设置对比度控制
  OLED_WriteCmd(0xCF);

  OLED_WriteCmd(0xD9); // 设置预充电周期
  OLED_WriteCmd(0xF1);

  OLED_WriteCmd(0xDB); // 设置VCOMH取消选择级别
  OLED_WriteCmd(0x30);

  OLED_WriteCmd(0xA4); // 设置整个显示打开/关闭

  OLED_WriteCmd(0xA6); // 设置正常/倒转显示

  OLED_WriteCmd(0x8D); // 设置充电泵
  OLED_WriteCmd(0x14);

  OLED_WriteCmd(0xAF); // 开启显示

  OLED_Clear(0); // OLED清屏
}