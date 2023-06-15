#include "led.h"
// 延时函数
void delay_ms(int ms)
{
    int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 1800; j++)
            ;
}
// led灯设置初始化
void all_led_init()
{
    RCC |= (0x3 << 4);
    // led1
    GPIOE->moder &= (~(0X3 << 20));
    GPIOE->moder |= (0X1 << 20);
    GPIOE->otyper &= (~(0X1 << 10));
    GPIOE->ospeedr &= (~(0X3 << 20));
    GPIOE->pupdr &= (~(0X3 << 20));
    // led2
    GPIOF->moder &= (~(0X3 << 20));
    GPIOF->moder |= (0X1 << 20);
    GPIOF->otyper &= (~(0X1 << 10));
    GPIOF->ospeedr &= (~(0X3 << 20));
    GPIOF->pupdr &= (~(0X3 << 20));
    // led3
    GPIOE->moder &= (~(0X3 << 16));
    GPIOE->moder |= (0X1 << 16);
    GPIOE->otyper &= (~(0X1 << 8));
    GPIOE->ospeedr &= (~(0X3 << 16));
    GPIOE->pupdr &= (~(0X3 << 16));
}
void all_led_flash()
{
    // LED1亮
    GPIOE->odr |= (0X1 << 10);
    GPIOF->odr &= (~(0X1 << 10));
    GPIOE->odr &= (~(0X1 << 8));
    delay_ms(1000);
    // LED2亮
    GPIOE->odr &= (~(0X1 << 10));
    GPIOF->odr |= (0X1 << 10);
    GPIOE->odr &= (~(0X1 << 8));
    delay_ms(1000);

    // LED3亮
    GPIOE->odr &= (~(0X1 << 10));
    GPIOF->odr &= (~(0X1 << 10));
    GPIOE->odr |= (0X1 << 8);
    delay_ms(1000);
}