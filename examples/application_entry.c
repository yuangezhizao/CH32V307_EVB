#include "tos_k.h"

#define TASK1_STK_SIZE 1024
k_task_t task1;
uint8_t task1_stk[TASK1_STK_SIZE];

#define TASK2_STK_SIZE 1024
k_task_t task2;
uint8_t task2_stk[TASK2_STK_SIZE];

#define TASK3_STK_SIZE 4096
k_task_t task3;
uint8_t task3_stk[TASK3_STK_SIZE];

#define MESSAGE_MAX 50
k_msg_q_t rgb_data_msg_q;
uint8_t rgb_data_msg_pool[MESSAGE_MAX];

void ws2812_spi_entry(void *arg)
{
    printf("ws2812_spi_entry entered!\r\n");
    ws2812_spi();
}

void lcd_show_RTC_entry(void *arg)
{
    printf("lcd_show_RTC entered!\r\n");
    lcd_show_RTC();
}

void mqtt_task_entry(void *arg)
{
    printf("mqtt_task_entry entered!\r\n");
    mqtt_task();
}

void application_entry(void *arg)
{
    exti_int_init();
    sg90_init();
    sunset_light_init();
    RTC_Init();
    user_led_1_toggle();
    lcd_show_image();

    tos_msg_q_create(&rgb_data_msg_q, rgb_data_msg_pool, MESSAGE_MAX);

    tos_task_create(&task1, "task1", ws2812_spi_entry, NULL, 3, task1_stk, TASK1_STK_SIZE, 0);   // Create task1
    tos_task_create(&task2, "task2", lcd_show_RTC_entry, NULL, 4, task2_stk, TASK2_STK_SIZE, 0); // Create task2
    tos_task_create(&task3, "task3", mqtt_task_entry, NULL, 3, task3_stk, TASK3_STK_SIZE, 0);    // Create task3

    printf("application finished!\r\n");
}
