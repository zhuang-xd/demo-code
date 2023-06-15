#include "nixie_light.h"
unsigned char nums[10] = { 0xFC, 0x60, 0xDA, 0xF2, 0x66, 0xB6, 0xBE, 0xE0, 0xFE, 0xF6 };

void delay_ms(int ms)
{
    int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 1800; j++)
            ;
}

/**
 * @brief    数码管显示
 * @param
 * @return
 * @note     0  0 0 0 每轮只显示4个数字，之一显示到 9 9 9 9
 */
void exam1()
{
    unsigned char i;
    for (i = 0; i < 10; i++) {
        SPI_write(0xFF); // 位选
        SPI_write(nums[i]); // 段选
        NSS_OUTPUT_L();
        delay_ms(1);
        NSS_OUTPUT_H();
        delay_ms(1000);
    }
}

/**
 * @brief    数码管显示
 * @param
 * @return
 * @note     1 2 3 4 每次只显示1位
 */
void exam2()
{
    unsigned char i;
    for (i = 0; i < 4; i++) {
        SPI_write(0x80 >> i); // 位选
        SPI_write(nums[i + 1]); // 段选
        NSS_OUTPUT_L();
        delay_ms(1);
        NSS_OUTPUT_H();
        delay_ms(1000);
    }
}