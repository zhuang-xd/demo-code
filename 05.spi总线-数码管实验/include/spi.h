#ifndef __SPI_H__
#define __SPI_H__

#include "stm32mp1xx_gpio.h"
#include "stm32mp1xx_rcc.h"
// MOSI对应的引脚输出高低电平的信号
#define  MOSI_OUTPUT_H()	do{GPIOE->ODR |= (0x1 << 14);}while(0)
#define  MOSI_OUTPUT_L()    do{GPIOE->ODR &= (~(0x1 << 14));}while(0)

// 对应595芯片的锁存引脚输出高低电平
#define  NSS_OUTPUT_H()	    do{GPIOE->ODR |= (0x1 << 11);}while(0)
#define  NSS_OUTPUT_L()     do{GPIOE->ODR &= (~(0x1 << 11));}while(0)
	
// 时钟信号对应的引脚输出高低电平
#define  SCK_OUTPUT_H()     do{GPIOE->ODR |= (0x1 << 12);}while(0)
#define  SCK_OUTPUT_L()     do{GPIOE->ODR &= (~(0x1 << 12));}while(0)

/*
 * 函数功能： SPI初始化函数，推挽输出，高速，禁止上拉和下拉
 * 函数参数：无
 * 函数返回值：无
*/
void SPI_init(void);
/*
 * 函数功能：SPI发送数据的函数
 * 函数参数：dat : 要发送的数据
 * 函数返回值：无
 *
*/
void SPI_write(unsigned char dat);


#endif  // __SPI_H__
