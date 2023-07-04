#include "stm32mp1xx_gpio.h"
#include "stm32mp1xx_uart.h"
// UART4_TX : PG11  AF6
// UART4_RX : PB2   AF8
void __uart_init()
{
   	// GPIOB2 设置为复用功能
   	GPIOB->MODER &= (~(0x3 << 4));
   	GPIOB->MODER |= (0x2 << 4);
	GPIOB->AFRL &= (~(0xF << 8));
	GPIOB->AFRL |= (0x8 << 8);
	// GPIOG11 设置为复用功能 
	GPIOG->MODER &= (~(0x3 << 22));
	GPIOG->MODER |= (0x2 << 4);
	GPIOG->AFRH &= (~(0xF << 12));
	GPIOG->AFRH |= (0x6 << 12);
	// 设置数据长度为8位   USART_CR1  
	USART4->CR1 &= (~(0x1 << 28));
	USART4->CR1 &= (~(0x1 << 12));
	// 采样率为16位 
	USART4->CR1 &= (~(0x1 << 15));
	// 设置波特率为115200bps 
	USART4->PRESC &= (~(0xF << 0));
	USART4->BRR = 0xD0;
	// 停止位1位
	USART4->CR2 &= (~(0x3 << 12));
	// 使能USART串口 
	USART4->CR1 |= (0x1 << 0);
	// 使能USART的发送或者接收功能 
	USART4->CR1 |= (0x1 << 3);
	USART4->CR1 |= (0x1 << 2);

}

void __uart_putc(const char data)
{
	while (!(USART4->ISR & (0x1 << 7)));
	USART4->TDR = data;
	if (data == '\n')
		__uart_putc('\r');
}
void __uart_puts(const  char  *pstr)
{
	while(*pstr != '\0')
		__uart_putc(*pstr++);
}

char __uart_getc(void)
{
	char ch;
	while(!(USART4->ISR & (0x1 << 5)));
	ch = (char)USART4->RDR;
	return ch;
}
