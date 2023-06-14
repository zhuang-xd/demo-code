#ifndef __KEY_H__
#define __KEY_H__

#include "stm32mp1xx_rcc.h"
#include "stm32mp1xx_gpio.h"
#include "stm32mp1xx_exti.h"
#include "stm32mp1xx_gic.h"

typedef unsigned int uint32_t;

// 下降沿启用/禁止
typedef enum {
    DISABLED,
    ENABLED
}desedge_t;

// 按键相关的寄存器初始化
void key_init();

#endif
