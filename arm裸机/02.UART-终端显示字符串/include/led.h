#ifndef __LED_H__
#define __LED_H__

//定义寄存器组织结构体
typedef struct{
    unsigned int moder;
    unsigned int otyper;
    unsigned int ospeedr;
    unsigned int pupdr;
    unsigned int idr;
    unsigned int odr;
}gpio_t;
//定义GPIOE和GPIOF
#define GPIOE ((gpio_t *)0X50006000)
#define GPIOF ((gpio_t *)0X50007000)
#define RCC (*(volatile unsigned int *)0X50000A28)
void delay_ms(int ms);
 void all_led_init();
 void  all_led_flash();
#endif