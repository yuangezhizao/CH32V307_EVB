#include "tos_k.h"

#define ARR 200 - 1
#define PSC 14400 - 1

void init_TIM4_PWMOut(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE); //使能GPIOD外设时钟和TIM4时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;        //配置PD12引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;   //设置为复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //设置输出速度：50MHz
    GPIO_Init(GPIOD, &GPIO_InitStructure);            // GPIO初始化

    TIM_TimeBaseInitStructure.TIM_Period = ARR;                     //指定下次更新事件时要加载到活动自动重新加载寄存器中的周期值。
    TIM_TimeBaseInitStructure.TIM_Prescaler = PSC;                  //指定用于划分TIM时钟的预分频器值。
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //时钟分频因子
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM计数模式，向上计数模式
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStructure);             //根据指定的参数初始化TIMx的时间基数单位

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; // PWM输出使能
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
    TIM_OC1Init(TIM4, &TIM_OCInitStructure); //根据TIM_OCInitStruct中指定的参数初始化TIM1 Channel1。

    // TIM_CtrlPWMOutputs(TIM4, ENABLE);                  //启用定时器1PWM输出
    // TIM_ARRPreloadConfig(TIM4, ENABLE);                //使能ARR上TIM1外设预加载寄存器
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Disable); //使能CCR1上的TIM1外设预加载寄存器
    TIM_Cmd(TIM4, ENABLE);                             //使能TIM1
}

void application_entry(void *arg)
{
    init_TIM4_PWMOut();
    while (1)
    {
        TIM_SetCompare1(TIM4, 195); // 0度
        Delay_Ms(1000);

        TIM_SetCompare1(TIM4, 190); // 45度
        Delay_Ms(1000);

        TIM_SetCompare1(TIM4, 185); // 90度
        Delay_Ms(1000);

        TIM_SetCompare1(TIM4, 180); // 135度
        Delay_Ms(1000);

        TIM_SetCompare1(TIM4, 175); // 180度
        Delay_Ms(1000);
    }
}
