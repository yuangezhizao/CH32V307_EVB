#include "sdio.h"
#include "lcd.h"
//#include "pic.h"

u8 buf[512];
u8 Readbuf[512];

/*********************************************************************
 * @fn      show_sdcard_info
 *
 * @brief   SD Card information.
 *
 * @return  none
 */
void show_sdcard_info(void)
{
    switch (SDCardInfo.CardType)
    {
    case SDIO_STD_CAPACITY_SD_CARD_V1_1:
        printf("Card Type:SDSC V1.1\r\n");
        break;
    case SDIO_STD_CAPACITY_SD_CARD_V2_0:
        printf("Card Type:SDSC V2.0\r\n");
        break;
    case SDIO_HIGH_CAPACITY_SD_CARD:
        printf("Card Type:SDHC V2.0\r\n");
        break;
    case SDIO_MULTIMEDIA_CARD:
        printf("Card Type:MMC Card\r\n");
        break;
    }
    printf("Card ManufacturerID:%d\r\n", SDCardInfo.SD_cid.ManufacturerID);
    printf("Card RCA:%d\r\n", SDCardInfo.RCA);
    printf("Card Capacity:%d MB\r\n", (u32)(SDCardInfo.CardCapacity >> 20));
    printf("Card BlockSize:%d\r\n\r\n", SDCardInfo.CardBlockSize);
}

/* sdcard test */
void SD_Card_test(void)
{
    u32 i;
    u32 Sector_Nums;

    while (SD_Init())
    {
        printf("SD Card Error!\r\n");
        delay_ms(1000);
    }

    show_sdcard_info();

    printf("SD Card OK\r\n");
    Sector_Nums = ((u32)(SDCardInfo.CardCapacity >> 20)) / 2;
    printf("Sector_Nums:%d\r\n", Sector_Nums);

    for (i = 0; i < 512; i++)
    {
        buf[i] = i;
    }

    for (i = 0; i < Sector_Nums / 2; i++)
    {
        if (SD_WriteDisk(buf, i, 1))
        {
            printf("Wr %d sector fail\n", i);
        }
        if (SD_ReadDisk(Readbuf, i, 1))
        {
            printf("Rd %d sector fail\n", i);
        }

        if (memcmp(buf, Readbuf, 512))
        {
            printf(" %d sector Verify fail\n", i);
            LCD_ShowString(16, 16 + 16 + 16 + 16, "SD Sector:FAIL", WHITE, BLACK, 16, 0);
            break;
        }
    }
    if (i == Sector_Nums / 2)
    {
        LCD_ShowString(16, 16 + 16 + 16 + 16 + 16, "SD: OK", WHITE, BLACK, 16, 0);
        printf("SD OK\r\n");
    }
    else
    {
        LCD_ShowString(16, 16 + 16 + 16 + 16 + 16, "SD ALL: FAIL", WHITE, BLACK, 16, 0);
    }
}

void write_one_picture_to_sd_card(const u8 image[], u8 pic_num)
{
    u16 each_sector;
    u32 current_sector;

    for (current_sector = pic_num * (sizeof(image) / 512); current_sector < (pic_num + 1) * (sizeof(image) / 512); current_sector++)
    {
        for (each_sector = 0; each_sector < 512; each_sector++)
        {
            buf[each_sector] = image[current_sector * 512 + each_sector];
            // printf("each_sector=%d, gImage_1=%d\r\n", each_sector, current_sector * 512 + each_sector);
        }
        if (SD_WriteDisk(buf, current_sector, 1))
        {
            printf("SD_WriteDisk error\r\n");
        }
        printf("pic_num=%d, current_sector=%d\r\n", pic_num, current_sector);
    }
    printf("write_pictures_to_sd_card\r\n");
}

// void write_pictures_to_sd_card()
//{
//     write_one_picture_to_sd_card(gImage_0, 0);
//     LCD_ShowPicture(0, 0, 120, 120, gImage_0);
//     printf("LCD_ShowPicture [gImage_0]\r\n");
//
//
//     write_one_picture_to_sd_card(gImage_1, 1);
//     LCD_ShowPicture(120, 0, 120, 120, gImage_1);
//     printf("LCD_ShowPicture [gImage_1]\r\n");
//
// }

// void read_one_picture_from_sd_card(unsigned char* image, u8 pic_num)
// {
//     u16 each_sector;
//     u32 current_sector;
//     u32 image_offset;
//
//     for (image_offset=0, current_sector = pic_num * (sizeof(image) / 512); current_sector < (pic_num + 1) * (sizeof(image) / 512); current_sector++)
//     {
//         if (SD_ReadDisk(buf, current_sector, 1))
//         {
//             printf("SD_ReadDisk error\r\n");
//         }
//         for (each_sector = 0; each_sector < 512; each_sector++)
//         {
//             image[image_offset] = buf[each_sector];
//             //            printf("each_sector=%d, image_offset=%d\r\n", each_sector, image_offset);
//             image_offset++;
//         }
//         printf("pic_num=%d, current_sector=%d, image_offset=%d\r\n", pic_num, current_sector, image_offset);
//     }
// }

void read_pictures_from_sd_card()
{
    unsigned char image[28800];
    u8 pic_num = 0;
    //    read_one_picture_from_sd_card(image, pic_num);
    //
    //    printf("read_picture_from_sd_card [%d]\r\n", pic_num);
    //    LCD_ShowPicture(0, 0, 120, 120, image);
    //    printf("LCD_ShowPicture [%d]\r\n", pic_num);

    u16 each_sector;
    u32 current_sector;
    u32 image_offset;

    for (image_offset = 0, current_sector = pic_num * (sizeof(image) / 512); current_sector < (pic_num + 1) * (sizeof(image) / 512); current_sector++)
    {
        if (SD_ReadDisk(buf, current_sector, 1))
        {
            printf("SD_ReadDisk error\r\n");
        }
        for (each_sector = 0; each_sector < 512; each_sector++)
        {
            image[image_offset] = buf[each_sector];
            //            printf("each_sector=%d, image_offset=%d\r\n", each_sector, image_offset);
            image_offset++;
        }
        printf("pic_num=%d, current_sector=%d, image_offset=%d\r\n", pic_num, current_sector, image_offset);
    }

    printf("read_picture_from_sd_card [%d]\r\n", pic_num);
    LCD_ShowPicture(0, 0, 120, 120, image);
    printf("LCD_ShowPicture [%d]\r\n", pic_num);

    pic_num = 1;

    for (image_offset = 0, current_sector = pic_num * (sizeof(image) / 512); current_sector < (pic_num + 1) * (sizeof(image) / 512); current_sector++)
    {
        if (SD_ReadDisk(buf, current_sector, 1))
        {
            printf("SD_ReadDisk error\r\n");
        }
        for (each_sector = 0; each_sector < 512; each_sector++)
        {
            image[image_offset] = buf[each_sector];
            //            printf("each_sector=%d, image_offset=%d\r\n", each_sector, image_offset);
            image_offset++;
        }
        printf("pic_num=%d, current_sector=%d, image_offset=%d\r\n", pic_num, current_sector, image_offset);
    }

    printf("read_picture_from_sd_card [%d]\r\n", pic_num);
    LCD_ShowPicture(120, 0, 120, 120, image);
    printf("LCD_ShowPicture [%d]\r\n", pic_num);
}

void my_SD_Card(void)
{
    u32 Sector_Nums;

    while (SD_Init())
    {
        printf("SD Card Error!\r\n");
        delay_ms(1000);
    }

    //     if (SD_InitializeCards())
    //     {
    //         printf("SD_InitializeCards fail\r\n");
    //     }
    //     else
    //     {
    //         printf("SD Card Initialize OK\r\n");
    //     }

    show_sdcard_info();

    printf("SD Card OK\r\n");
    Sector_Nums = ((u32)(SDCardInfo.CardCapacity >> 20)) / 2;
    printf("Sector_Nums:%d\r\n", Sector_Nums);

    //    LCD_ShowPicture(0, 0, 120, 120, gImage_0);
    //    LCD_ShowPicture(120, 0, 120, 120, gImage_1);

    //    write_pictures_to_sd_card();
    read_pictures_from_sd_card();
}

void application_entry(void *arg)
{
    my_SD_Card();
    while (1)
    {
    }
}
