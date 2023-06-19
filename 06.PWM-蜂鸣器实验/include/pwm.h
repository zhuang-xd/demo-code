#ifndef _PWM_H_
#define _PWM_H_

#include "stm32mp1xx_rcc.h"
#include "stm32mp1xx_gpio.h"
#include "stm32mp1xx_tim.h"

// 蜂鸣器初始化
void hal_tim4_beep_init();

// 风扇始化
void hal_tim1_fan_init();
// 马达始化
void hal_tim16_monitor_init();

#endif //_PWM_H_