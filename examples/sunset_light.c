#include "debug.h"

extern void sunset_light_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

// void application_entry(void *arg)
// {
//     sunset_light_init();
//     while (1)
//     {
//         turn_on_sunset_light();
//         Delay_Ms(1000);
//         turn_off_sunset_light();
//         Delay_Ms(1000);
//     }
// }
