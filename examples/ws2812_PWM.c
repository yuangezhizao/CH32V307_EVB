#include <ws2812_PWM.h>

void application_entry(void *arg)
{
    // uart_init(115200);
    // delay_init();
    WS281x_Init();
    printf("SystemClk:%d\r\n", SystemCoreClock);

    while (1)
    {
        WS281x_ColorWipe(WS281x_Color(255, 0, 0), 50); // Red
        Delay_Ms(1000);
        WS281x_ColorWipe(WS281x_Color(0, 255, 0), 50); // Green
        Delay_Ms(1000);
        WS281x_ColorWipe(WS281x_Color(200, 255, 0), 50); //黄色
        Delay_Ms(1000);
        WS281x_ColorWipe(WS281x_Color(100, 255, 50), 50); //冰蓝
        Delay_Ms(1000);
        WS281x_ColorWipe(WS281x_Color(155, 255, 0), 50); //嫩绿色
        Delay_Ms(1000);

        // for (i = 0; i < PIXEL_NUM; ++i)
        // {
        //     ws281x_setPixelColor(i, ws281x_color(255, 250, 0));
        //     ws281x_show();
        //     Delay_Ms(100);
        // }
    }
}
