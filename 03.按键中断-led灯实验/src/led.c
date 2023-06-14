#include "led.h"

/** 
* @brief    led1、led2、led3初始化
* @param    无
* @return   无
*/
void led_init()
{
    RCC->MP_AHB4ENSETR |= (0x3 << 4); // GPIOE和GPIOF RCC使能

    // LED1 --> PE10
    GPIOE->MODER &= (~(0X3 << 20));
    GPIOE->MODER |= (0X1 << 20); // 输出模式
    GPIOE->OTYPER &= (~(0X1 << 10)); // 推挽输出
    GPIOE->OSPEEDR &= (~(0X3 << 20)); // 低速
    GPIOE->PUPDR &= (~(0X3 << 20)); // 不设置上/下拉电阻

    // LED2 --> PF10
    GPIOF->MODER &= (~(0X3 << 20));
    GPIOF->MODER |= (0X1 << 20); // 输出模式
    GPIOF->OTYPER &= (~(0X1 << 10)); // 推挽输出
    GPIOF->OSPEEDR &= (~(0X3 << 20)); // 低速
    GPIOF->PUPDR &= (~(0X3 << 20)); // 不设置上/下拉电阻

    // LED3 --> PE8
    GPIOE->MODER &= (~(0X3 << 16));
    GPIOE->MODER |= (0X1 << 16); // 输出模式
    GPIOE->OTYPER &= (~(0X1 << 8)); // 推挽输出
    GPIOE->OSPEEDR &= (~(0X3 << 16)); // 低速
    GPIOE->PUPDR &= (~(0X3 << 16)); // 不设置上/下拉电阻
}


/** 
* @brief    切换led状态
* @param    led_id LED灯的id号
* @return   无
*/
void toggle_led(int led_id)
{
    switch (led_id) {
    case 1:
        GPIOE->ODR ^= (0x1 << 10); // LED1 状态取反
        break;
    case 2:
        GPIOF->ODR ^= (0x1 << 10); // LED2 状态取反
        break;
    case 3:
        GPIOE->ODR ^= (0x1 << 8); // LED3 状态取反
        break;
    }
}


