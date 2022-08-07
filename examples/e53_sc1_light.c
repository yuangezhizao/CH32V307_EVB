#include "tos_k.h"

extern void init_e53_sc1_light(void)
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
//     init_e53_sc1_light();
//     while (1)
//     {
//         GPIO_SetBits(GPIOC, GPIO_Pin_10);
//         Delay_Ms(1000);
//         GPIO_ResetBits(GPIOC, GPIO_Pin_10);
//         Delay_Ms(1000);
//     }
// }
