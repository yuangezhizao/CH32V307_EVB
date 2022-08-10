#include "ws2812_SPI.h"

// int8_t i;

extern void ws2812_spi(void)
{
    ws281x_init();
    while (1)
    {
        ws281x_colorWipe(ws281x_color(255, 0, 0), 50); // Red
        tos_sleep_ms(1000);
        ws281x_colorWipe(ws281x_color(0, 255, 0), 50); // Green
        tos_sleep_ms(1000);
        ws281x_colorWipe(ws281x_color(200, 255, 0), 50); //黄色
        tos_sleep_ms(1000);
        ws281x_colorWipe(ws281x_color(100, 255, 50), 50); //冰蓝
        tos_sleep_ms(1000);
        ws281x_colorWipe(ws281x_color(155, 255, 0), 50); //嫩绿色
        tos_sleep_ms(1000);

        // for (i = 0; i < PIXEL_NUM; ++i)
        // {
        //     ws281x_setPixelColor(i, ws281x_color(255, 250, 0));
        //     ws281x_show();
        //     Delay_Ms(100);
        //

        // Send a theater pixel chase in...
        ws281x_theaterChase(ws281x_color(127, 127, 127), 50); // White
        tos_sleep_ms(1000);
        ws281x_theaterChase(ws281x_color(127, 0, 0), 50); // Red
        tos_sleep_ms(1000);
        ws281x_theaterChase(ws281x_color(0, 0, 127), 50); // Blue
        tos_sleep_ms(1000);

        ws281x_rainbow(20);
        tos_sleep_ms(1000);
        ws281x_rainbowCycle(20);
        tos_sleep_ms(1000);
        ws281x_theaterChaseRainbow(20);
        tos_sleep_ms(1000);
    }
}
