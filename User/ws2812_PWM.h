#ifndef __WS281X_PWM_H
#define __WS281X_PWM_H

//#include "stm32f10x.h"
#include "debug.h"

#define WS281x_GPIO_CLK RCC_APB2Periph_GPIOE
#define WS281x_GPIO_CLK_FUNCTION RCC_APB2PeriphClockCmd
#define WS281x_GPIOx GPIOE
#define WS281x_GPIO_PIN GPIO_Pin_9

#define WS281x_TIMx TIM1
#define WS281x_TIM_CLK RCC_APB2Periph_TIM1
#define WS281x_TIM_CLK_FUNCTION RCC_APB2PeriphClockCmd
#define WS281x_TIM_DMA_SOURCE TIM_DMA_CC1

#define WS281x_DMA_CLK RCC_AHBPeriph_DMA1
#define WS281x_DMA_CLK_FUNCTION RCC_AHBPeriphClockCmd
#define WS281x_DMA_PERIPH_ADDR (uint32_t) & (TIM1->CH1CVR)
#define WS281x_DMAx_CHANNELx DMA1_Channel2
#define WS281x_DMA_FLAG DMA1_FLAG_TC2

#define PIXEL_NUM 30
#define GRB 24 // 3*8

#define WS_HIGH 45
#define WS_LOW 30

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
#endif /* __WS281X_PWM_H */
