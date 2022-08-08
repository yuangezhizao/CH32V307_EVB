/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : Main program body.
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * SPDX-License-Identifier: Apache-2.0
 *******************************************************************************/

/*
 *@Note
 GPIO例程：
 PE2推挽输出。
*/

//#include "debug.h"
#include "tos_k.h"

/* Global define */

/* Global Variable */

/*********************************************************************
 * @fn      GPIO_Toggle_INIT
 *
 * @brief   Initializes GPIOA.0
 *
 * @return  none
 */
void GPIO_Toggle_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void application_entry(void *arg)
{
    //    u8 i = 0;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    //    Delay_Init();
    //    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);

    printf("GPIO Toggle TEST\r\n");
    //    EXTI0_INT_INIT();

    GPIO_Toggle_INIT();
    TIM6_Init(5000 - 1, 14400 - 1);
    TIM6_INT_INIT();

    while (1)
    {
        //        Delay_Ms(250);
        //        GPIO_WriteBit(GPIOA, GPIO_Pin_2, (i == 0) ? (i = Bit_SET) : (i = Bit_RESET));
    }
}
