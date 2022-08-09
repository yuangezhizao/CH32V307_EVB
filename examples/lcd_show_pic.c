#include "debug.h"
#include "pic.h"

extern void lcd_show_image(void *arg)
{
    // u8 i, j;
    // float t = 0;

    // LCD_ShowString(0, 0, "Welcome TencentOS", WHITE, BLACK, 32, 0);
    // LCD_ShowString(0, 40, "LCD_W:", WHITE, BLACK, 16, 0);
    // LCD_ShowIntNum(48, 40, LCD_W, 3, WHITE, BLACK, 16);
    // LCD_ShowString(80, 40, "LCD_H:", WHITE, BLACK, 16, 0);
    // LCD_ShowIntNum(128, 40, LCD_H, 3, WHITE, BLACK, 16);
    // LCD_ShowString(80, 40, "LCD_H:", WHITE, BLACK, 16, 0);
    // LCD_ShowString(0, 70, "Increaseing Nun:", WHITE, BLACK, 16, 0);
    // LCD_ShowFloatNum1(128, 70, t, 4, WHITE, BLACK, 16);
    // t += 0.11;
    // for (j = 0; j < 3; j++)
    // {
    //     for (i = 0; i < 6; i++)
    //     {
    //         LCD_ShowPicture(40 * i, 120 + j * 40, 40, 40, gImage_1);
    //     }
    // }
    LCD_ShowPicture(0, 0, 120, 120, gImage_0);
    LCD_ShowPicture(120, 0, 120, 120, gImage_1);

    printf("lcd_show_image finish\r\n");
}
