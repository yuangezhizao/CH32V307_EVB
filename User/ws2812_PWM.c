#include <ws2812_PWM.h>
#include "debug.h"

uint16_t pixelBuffer[PIXEL_NUM][GRB];

/* CH1CVR register Definition */
//#define TIM1_CH1CVR_ADDRESS    0x40012C34

void WS281x_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
  TIM_OCInitTypeDef TIM_OCInitStruct;
  DMA_InitTypeDef DMA_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

  GPIO_PinRemapConfig(GPIO_FullRemap_TIM1, ENABLE);

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

  GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOE, &GPIO_InitStruct);

  TIM_TimeBaseInitStruct.TIM_Period = 180 - 1; // 144M / 180 = 800K
  TIM_TimeBaseInitStruct.TIM_Prescaler = 0;
  TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
  //    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
  TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);

  TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStruct.TIM_Pulse = 0;
  TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStruct.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OC1Init(TIM1, &TIM_OCInitStruct);
  TIM_CtrlPWMOutputs(TIM1, ENABLE); // MOE 主输出使能

  /*必须有下面该语句，否则时序会错乱*/
  //  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); // CH1预装载使能

  TIM_Cmd(TIM1, DISABLE);
  TIM_DMACmd(TIM1, TIM_DMA_CC1, ENABLE);

  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) & (TIM1->CH1CVR);
  //  DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)TIM1_CH1CVR_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)pixelBuffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = PIXEL_NUM * GRB;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

  DMA_Init(DMA1_Channel2, &DMA_InitStructure);
  DMA_Cmd(DMA1_Channel2, DISABLE);

  /*关闭所有灯珠*/
  WS281x_CloseAll();
  Delay_Ms(100);
}

void WS281x_CloseAll(void)
{
  uint16_t i;
  uint8_t j;

  for (i = 0; i < PIXEL_NUM; ++i)
  {
    for (j = 0; j < 24; ++j)
    {
      pixelBuffer[i][j] = WS_LOW;
    }
  }
  WS281x_Show();
}

uint32_t WS281x_Color(uint8_t red, uint8_t green, uint8_t blue)
{
  return green << 16 | red << 8 | blue;
}

void WS281x_SetPixelColor(uint16_t n, uint32_t GRBColor)
{
  uint8_t i;
  if (n < PIXEL_NUM)
  {
    for (i = 0; i < GRB; i++)
    {
      pixelBuffer[n][i] = ((GRBColor << i) & 0x800000) ? WS_HIGH : WS_LOW;
    }
  }
}

void WS281x_SetPixelRGB(uint16_t n, uint8_t red, uint8_t green, uint8_t blue)
{
  uint8_t i;

  if (n < PIXEL_NUM)
  {
    for (i = 0; i < GRB; ++i)
    {
      pixelBuffer[n][i] = (((WS281x_Color(red, green, blue) << i) & 0X800000) ? WS_HIGH : WS_LOW);
    }
  }
}

////下面函数中语句执行的顺序相当关键，不能错。
////从示波器看，非当前的执行顺序，最后一个灯的波形序列会多。
// void WS281x_Show(void)
//{
//   DMA_SetCurrDataCounter(DMA1_Channel2, PIXEL_NUM * GRB);
//   TIM_Cmd(TIM1, ENABLE);
//   DMA_Cmd(DMA1_Channel2, ENABLE);
//   while (DMA_GetFlagStatus(DMA1_FLAG_TC1) != SET)
//     ;
//   TIM_Cmd(TIM1, DISABLE);
//   // TIM_SetCompare1(TIM1, 0); 该句不能加，加上灯不会按正常流程显示
//   DMA_Cmd(DMA1_Channel2, DISABLE);
//
//   DMA_ClearFlag(DMA1_FLAG_TC1);
// }

/*
 * bref ：经测试这么写显示函数，可以使用。
 *        TIM_OC1PreloadConfig函数如果只放在初始化函数中使能，灯珠会错位显示。如果没有该函数，在动态变换过程中灯珠的颜色并不能正常显示。
 */
void WS281x_Show(void)
{
  DMA_SetCurrDataCounter(DMA1_Channel2, (uint16_t)(PIXEL_NUM * GRB)); //设定DMA要传输数据的大小
  DMA_Cmd(DMA1_Channel2, ENABLE);

  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable); // CH1预装载器使能
  TIM_Cmd(TIM2, ENABLE);

  while (DMA_GetFlagStatus(DMA1_FLAG_TC2) != SET)
    ; //等待传输完成

  DMA_Cmd(DMA1_Channel2, DISABLE);
  TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Disable); //关闭CH1预装载器
  TIM_Cmd(TIM2, DISABLE);
  DMA_ClearFlag(DMA1_FLAG_TC2);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t WS281x_Wheel(uint8_t wheelPos)
{
  wheelPos = 255 - wheelPos;
  if (wheelPos < 85)
  {
    return WS281x_Color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if (wheelPos < 170)
  {
    wheelPos -= 85;
    return WS281x_Color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return WS281x_Color(wheelPos * 3, 255 - wheelPos * 3, 0);
}

// Fill the dots one after the other with a color
void WS281x_ColorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < PIXEL_NUM; i++)
  {
    WS281x_SetPixelColor(i, c);
    WS281x_Show();
    Delay_Ms(wait);
  }
}

void WS281x_Rainbow(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256; j++)
  {
    for (i = 0; i < PIXEL_NUM; i++)
    {
      WS281x_SetPixelColor(i, WS281x_Wheel((i + j) & 255));
    }
    WS281x_Show();
    Delay_Ms(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void WS281x_RainbowCycle(uint8_t wait)
{
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++)
  { // 5 cycles of all colors on wheel
    for (i = 0; i < PIXEL_NUM; i++)
    {
      WS281x_SetPixelColor(i, WS281x_Wheel(((i * 256 / PIXEL_NUM) + j) & 255));
    }
    WS281x_Show();
    Delay_Ms(wait);
  }
}

// Theatre-style crawling lights.
void WS281x_TheaterChase(uint32_t c, uint8_t wait)
{
  for (int j = 0; j < 10; j++)
  { // do 10 cycles of chasing
    for (int q = 0; q < 3; q++)
    {
      for (uint16_t i = 0; i < PIXEL_NUM; i = i + 3)
      {
        WS281x_SetPixelColor(i + q, c); // turn every third pixel on
      }
      WS281x_Show();

      Delay_Ms(wait);

      for (uint16_t i = 0; i < PIXEL_NUM; i = i + 3)
      {
        WS281x_SetPixelColor(i + q, 0); // turn every third pixel off
      }
    }
  }
}

// Theatre-style crawling lights with rainbow effect
void WS281x_TheaterChaseRainbow(uint8_t wait)
{
  for (int j = 0; j < 256; j++)
  { // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++)
    {
      for (uint16_t i = 0; i < PIXEL_NUM; i = i + 3)
      {
        WS281x_SetPixelColor(i + q, WS281x_Wheel((i + j) % 255)); // turn every third pixel on
      }
      WS281x_Show();

      Delay_Ms(wait);

      for (uint16_t i = 0; i < PIXEL_NUM; i = i + 3)
      {
        WS281x_SetPixelColor(i + q, 0); // turn every third pixel off
      }
    }
  }
}
