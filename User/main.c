/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : Main program body.
 *******************************************************************************/
#include "debug.h"
#include "tos_k.h"

#include "lcd_init.h"
#include "lcd.h"

void led_key_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GPIO_SetBits(GPIOE, GPIO_Pin_2);
    GPIO_SetBits(GPIOE, GPIO_Pin_3);
    GPIO_SetBits(GPIOE, GPIO_Pin_4);
    GPIO_SetBits(GPIOE, GPIO_Pin_5);
    /* key 1 2 3 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t get_key(void)
{
    uint16_t value = 0;
    if ((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) & GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) & GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)) == 0)
    {
        Delay_Ms(15);
        if ((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) & GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) & GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_8)) == 0)
        {
            value = GPIO_ReadInputData(GPIOA);
            if ((value & 0x1) == 0)
                return 1;
            else if ((value & 0x2) == 0)
                return 2;
            else if ((value & 0x100) == 0)
                return 3;
            else
                return 0;
        }
    }
    return 0;
}

#define APPLICATION_TASK_STK_SIZE 5120
k_task_t application_task;
__aligned(4) uint8_t application_task_stk[APPLICATION_TASK_STK_SIZE];

extern void application_entry(void *arg);

volatile uint8_t key = 0;

int main(void)
{
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%d\r\n", SystemCoreClock);
    led_key_init();
    LCD_Init();
    //    SPI_Flash_Init();
    printf("Welcome to TencentOS tiny(%s)\r\n", TOS_VERSION);
    LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
    LCD_ShowString(0, 0, "Welcome to TencentOS", WHITE, BLACK, 16, 0);

    tos_knl_init();
    tos_task_create(&application_task, "application_task", application_entry, NULL, 4, application_task_stk, APPLICATION_TASK_STK_SIZE, 0);
    tos_knl_start();
    printf("should not run at here!\r\n");

    while (!key)
    {
        key = get_key();
    }

    switch (key)
    {
    case 1:
        break;
    case 2:
        //            LCD_ShowPicture(0,0,240,240,read_pic_from_sd(1));
        //            LCD_Fill(0,0,LCD_W,LCD_H,BLACK);
        //                spi_flash_test();
        break;
    case 3:
        LCD_Fill(0, 0, LCD_W, LCD_H, BLACK);
        break;
    default:
        break;
    }

    while (1)
        ;
}
