#ifndef __LED_H__
#define __LED_H__

#include "stm32mp1xx_rcc.h"
#include "stm32mp1xx_gpio.h"

// led初始化
void led_init();
// led状态切换
void toggle_led(int led_id);

#endif



