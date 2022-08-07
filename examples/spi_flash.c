#include "tos_k.h"
#include "spi_flash.h"
#include "lcd.h"

const u8 TEXT_Buf[] = {"CH32V307 SPI FLASH W25Q64"};

void spi_flash_test(void)
{
    u8 datap[SIZE];
    u16 Flash_Model;
    // SPI_Flash_Init();
    Delay_Ms(50);

    Flash_Model = SPI_Flash_ReadID();

    switch (Flash_Model)
    {
    case W25Q80:
        printf("W25Q80 OK!\r\n");
        break;

    case W25Q16:
        printf("W25Q16 OK!\r\n");
        break;

    case W25Q32:
        printf("W25Q32 OK!\r\n");
        break;

    case W25Q64:
        printf("W25Q64 OK!\r\n");
        break;

    case W25Q128:
        printf("W25Q128 OK!\r\n");
        break;

    default:
        printf("Fail!\r\n");
        break;
    }
    printf("Start Read W25Qxx before erase....\r\n");
    SPI_Flash_Read(datap, 0x0, SIZE);
    for (int16_t i = 0; i < SIZE; i++)
    {
        // printf("%x ", datap[i]);
        printf("%s ", datap[i]);
    }
    printf("\r\n");

    printf("Start Erase W25Qxx....\r\n");
    SPI_Flash_Erase_Sector(0);

    Delay_Ms(500);

    printf("Start Read W25Qxx....\r\n");
    SPI_Flash_Read(datap, 0x0, SIZE);
    for (int16_t i = 0; i < SIZE; i++)
    {
        printf("%x ", datap[i]);
    }
    printf("\r\n");

    Delay_Ms(500);

    printf("Start Write W25Qxx....\r\n");
    SPI_Flash_Write((u8 *)TEXT_Buf, 0, SIZE);

    Delay_Ms(500);

    SPI_Flash_Read(datap, 0x0, SIZE);
    printf("%s\r\n", datap);

    if (memcmp(TEXT_Buf, datap, SIZE))
    {
        LCD_ShowString(16, 16 + 16 + 16, "SPI Flash: Fail", WHITE, BLACK, 16, 0);
        printf("SPI Flash: Fail\r\n");
    }
    else
    {
        LCD_ShowString(16, 16 + 16 + 16, "SPI Flash: OK", WHITE, BLACK, 16, 0);
        printf("SPI Flash: OK\r\n");
    }
}

void application_entry(void *arg)
{
    spi_flash_test();
    while (1)
    {
    }
}
