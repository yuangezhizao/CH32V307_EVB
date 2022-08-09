//#include "led.h"
//#include "delay.h"
//#include "key.h"
//#include "sys.h"
//#include "usart.h"
#include <ws2812_SPI.h>

int8_t i;

void application_entry(void *arg)
{
    // uart_init(115200);
    // delay_init();
    ws281x_init();
    printf("SystemClk:%d\r\n", SystemCoreClock);
    while (1)
    {
        ws281x_colorWipe(ws281x_color(255, 0, 0), 50); // Red
        Delay_Ms(1000);
        ws281x_colorWipe(ws281x_color(0, 255, 0), 50); // Green
        Delay_Ms(1000);
        ws281x_colorWipe(ws281x_color(200, 255, 0), 50); //黄色
        Delay_Ms(1000);
        ws281x_colorWipe(ws281x_color(100, 255, 50), 50); //冰蓝
        Delay_Ms(1000);
        ws281x_colorWipe(ws281x_color(155, 255, 0), 50); //嫩绿色
        Delay_Ms(1000);

        // for (i = 0; i < PIXEL_NUM; ++i)
        // {
        //     ws281x_setPixelColor(i, ws281x_color(255, 250, 0));
        //     ws281x_show();
        //     Delay_Ms(100);
        // }

        // Send a theater pixel chase in...
        ws281x_theaterChase(ws281x_color(127, 127, 127), 50); // White
        Delay_Ms(1000);
        ws281x_theaterChase(ws281x_color(127, 0, 0), 50); // Red
        Delay_Ms(1000);
        ws281x_theaterChase(ws281x_color(0, 0, 127), 50); // Blue
        Delay_Ms(1000);

        // ws281x_rainbow(20);
        ws281x_rainbowCycle(20);
        Delay_Ms(1000);
        ws281x_theaterChaseRainbow(200);
        Delay_Ms(1000);
    }
}
