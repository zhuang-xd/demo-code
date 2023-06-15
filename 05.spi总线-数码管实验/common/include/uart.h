#ifndef __UART_H__
#define __UART_H__

void __uart_init();

void __uart_putc(const char data);

void __uart_puts(const  char  *pstr);

char __uart_getc(void);

#endif
