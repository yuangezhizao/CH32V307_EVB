#ifndef __WS2812_SPI_H
#define __WS2812_SPI_H

//#include "stm32f10x.h"
#include "debug.h"

#define PIXEL_NUM 30

//硬件spi模拟ws2811时序（用spi的8位数据模拟ws281x的一位数据）
//要将系统时钟设置为56M，分频数设置为8，则SPI的通信频率为7M，传输一位数据的时间约为143纳秒（ns）
// 3*143 = 429ns   5*143 = 715ns  符合WS281X芯片的通信时序。
//  _____
// |     |___|   11111000  high level
//  ___
// |   |_____|   11100000  low level

// OK
// 1 80 0.111
// 7 FE 0.777
#define WS_LOW 0x80
#define WS_HIGH 0xFE

// 2 C0 0.222
// 6 FC 0.666
//#define WS_LOW 0xC0
//#define WS_HIGH 0xFC

// 3 E0 0.333
// 5 F8 0.555
//#define WS_LOW 0xE0
//#define WS_HIGH 0xF8

void ws281x_init(void);
void ws281x_closeAll(void);
void ws281x_rainbowCycle(uint8_t wait);
uint32_t ws281x_color(uint8_t red, uint8_t green, uint8_t blue);
void ws281x_setPixelColor(uint16_t n, uint32_t GRBcolor);
void ws281x_show(void);

void ws281x_theaterChase(uint32_t c, uint8_t wait);
void ws281x_colorWipe(uint32_t c, uint8_t wait);
void ws281x_rainbow(uint8_t wait);
void ws281x_theaterChaseRainbow(uint8_t wait);
uint32_t ColorHSV(uint16_t hue, uint8_t sat, uint8_t val);

#endif /* __WS2812_SPI_H */
