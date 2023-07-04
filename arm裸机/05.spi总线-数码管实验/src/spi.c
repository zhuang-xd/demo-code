#include "spi.h"
/* SPI4_NSS 	---->   PE11
 * SPI4_SCK     ---->   PE12
 * SPI4_MOSI    ---->   PE14
 * SPI4_MISO    ---->   PE13
 * */

/* 数码管的编码， 先发送低位，在发送高位
 * A B C D E F G DP
 * 1 1 1 1 1 1 0 0    0xFC   0
 * 0 1 1 0 0 0 0 0    0x60   1
 * 1 1 0 1 1 0 1 0    0xDA   2
 * 1 1 1 1 0 0 1 0    0xF2   3
 * 0 1 1 0 0 1 1 0    0x66   4
 * 1 0 1 1 0 1 1 0    0xB6   5
 * 1 0 1 1 1 1 1 0    0xBE   6
 * 1 1 1 0 0 0 0 0    0xE0   7
 * 1 1 1 1 1 1 1 0    0xFE   8
 * 1 1 1 1 0 1 1 0    0xF6   9
 * */
void delay_us1(unsigned int us)
{
    int i, j;
    for (i = 0; i < us; i++)
        for (j = 0; j < 1; j++)
            ;
}

void SPI_init(void)
{
    RCC->MP_AHB4ENSETR |= (0x1 << 4);
    // MOSI    PE14
    GPIOE->MODER &= (~(0x3 << 28));
    GPIOE->MODER |= (0x1 << 28);
    GPIOE->OTYPER &= (~(0x1 << 14));
    GPIOE->OSPEEDR &= (~(0x3 << 28));
    GPIOE->PUPDR &= (~(0x3 << 28));
    // MISO    PE13
    GPIOE->MODER &= (~(0x3 << 26));
    GPIOE->OSPEEDR &= (~(0x3 << 26));
    GPIOE->PUPDR &= (~(0x3 << 26));
    // SCK     PE12
    GPIOE->MODER &= (~(0x3 << 24));
    GPIOE->MODER |= (0x1 << 24);
    GPIOE->OTYPER &= (~(0x1 << 12));
    GPIOE->OSPEEDR &= (~(0x3 << 24));
    GPIOE->PUPDR &= (~(0x3 << 24));
    // NSS     PE11
    GPIOE->MODER &= (~(0x3 << 22));
    GPIOE->MODER |= (0x1 << 22);
    GPIOE->OTYPER &= (~(0x1 << 11));
    GPIOE->OSPEEDR &= (~(0x3 << 22));
    GPIOE->PUPDR &= (~(0x3 << 22));
    NSS_OUTPUT_L(); // 595芯片的锁存引脚拉低
    SCK_OUTPUT_L(); // SPI的时钟线拉低
}

/**
* @brief    SPI发送数据
* @param
* @return
* @note    通过for循环来依次发送数据的每一位。
           在每一次循环中，
                先判断当前数据的最低位是0还是1，
                然后将这一位数据通过MOSI线发送出去。
                接着，将数据向右移一位，准备发送下一位数据。
                在时钟信号的控制下，
                    通过NSS输出信号来选择从哪个从设备发送数据，
                    通过SCK时钟信号来控制数据的传输和采样。
            最后，通过for循环依次发送完所有的数据位。
*/
void SPI_write(unsigned char dat)
{
    unsigned char i;
    // 先发送低位，再发送高位
    for (i = 0; i < 8; i++) {
        // 发送数据的最低位
        if (dat & 0x01) {
            MOSI_OUTPUT_H(); // MOSI输出高电平
        } else {
            MOSI_OUTPUT_L(); // MOSI输出低电平
        }
        delay_us1(1000);
        // 移位寄存器向右移一位，准备发送下一位数据
        dat >>= 1;
        // 时钟信号上升沿到下降沿之间，数据被采样
        SCK_OUTPUT_L();
        delay_us1(10);
        // 时钟信号下降沿到上升沿之间，数据被传输
        SCK_OUTPUT_H();
        delay_us1(10);
        // 时钟信号上升沿到下降沿之间，NSS输出高电平
    }
}