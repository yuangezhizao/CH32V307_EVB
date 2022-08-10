#include "ws2812_SPI.h"
#include "tos_k.h"
#include "lcd.h"

extern rgb_data_msg_q;

void ws2812_spi(void)
{
    k_err_t err;
    void *msg_received;
    // int8_t i;

    ws281x_init();

    // for (i = 0; i < PIXEL_NUM; ++i)
    // {
    //     ws281x_setPixelColor(i, ws281x_color(255, 250, 0));
    //     ws281x_show();
    //     Delay_Ms(100);
    //

    while (K_TRUE)
    {
        err = tos_msg_q_pend(&rgb_data_msg_q, &msg_received, TOS_TIME_FOREVER);
        if (err == K_ERR_NONE)
        {
            printf("higher: msg incoming[%s]\n", (char *)msg_received);

            LCD_ShowString(16 * 5, 120 + 24, (char *)msg_received, PINK, BLACK, 16, 0);

            if (strcmp((char *)msg_received, "color_wipe") == 0)
            {
                printf("color_wipe type\r\n");

                ws281x_colorWipe(ws281x_color(255, 0, 0), 50); // Red
                tos_sleep_ms(500);
                ws281x_colorWipe(ws281x_color(0, 255, 0), 50); // Green
                tos_sleep_ms(500);
                ws281x_colorWipe(ws281x_color(200, 255, 0), 50); //黄色
                tos_sleep_ms(500);
                ws281x_colorWipe(ws281x_color(100, 255, 50), 50); //冰蓝
                tos_sleep_ms(500);
                ws281x_colorWipe(ws281x_color(155, 255, 0), 50); //嫩绿色
                tos_sleep_ms(500);
            }
            else if (strcmp((char *)msg_received, "theater_chase") == 0)
            {
                printf("theater_chase type\r\n");

                ws281x_theaterChase(ws281x_color(127, 127, 127), 50); // White
                tos_sleep_ms(500);
                ws281x_theaterChase(ws281x_color(127, 0, 0), 50); // Red
                tos_sleep_ms(500);
                ws281x_theaterChase(ws281x_color(0, 0, 127), 50); // Blue
                tos_sleep_ms(500);
            }
            else if (strcmp((char *)msg_received, "rainbow") == 0)
            {
                printf("rainbow type\r\n");
                ws281x_rainbow(20);
            }
            else if (strcmp((char *)msg_received, "rainbow_cycle") == 0)
            {
                printf("rainbow_cycle type\r\n");
                ws281x_rainbowCycle(20);
            }
            else if (strcmp((char *)msg_received, "theater_chase_rain") == 0)
            {
                printf("theater_chase_rain type\r\n");
                ws281x_theaterChaseRainbow(20);
                tos_sleep_ms(1000);
            }
            else
            {
                printf("Unknown type\r\n");
            }
        }
        ws281x_closeAll();
    }
}
