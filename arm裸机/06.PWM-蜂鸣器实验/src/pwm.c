#include "pwm.h"

// /**
//  * @brief   蜂鸣器初始化
//  * @param   无
//  * @return  无
//  */
// void hal_tim4_beep_init()
// {
//     /* RCC */
//     RCC->MP_AHB4ENSETR |= (0x1 << 1); // GPIOB = AHB4[1]使能
//     RCC->MP_APB1ENSETR |= (0x1 << 2); // TIM4_CH1 =  APB1[2]使能

//     /* GPIO */
//     GPIOB->MODER &= (~(0x3 << 12)); // 清零
//     GPIOB->MODER |= (0x2 << 12); // PB6设置复用模式
//     GPIOB->AFRL &= (~(0xf << 8)); // 清零
//     GPIOB->AFRL |= (0x2 << 8); // PB6设置 TIM4_CH1复用 AF2

//     /* TIM ---> 时钟频率 209 */
//     TIM4->PSC &= (~(0xffff) << 0);
//     TIM4->PSC |= (0xD0 << 0); // 设置分频寄存器 208
//     TIM4->ARR &= (~(0xffff) << 0);
//     TIM4->ARR |= (0x3E8 << 0); // 设置脉冲周期 1000
//     TIM4->CCR1 &= (~(0xffffffff) << 0);
//     TIM4->CCR1 |= (0x2Bc << 0); // 设置1个脉冲周期高电平700  即PWM占空 70%
//     // 设置比较寄存器
//     TIM4->CCMR1 &= (~(0x1 << 16));
//     TIM4->CCMR1 &= (~(0x7 << 4));
//     TIM4->CCMR1 |= (0x3 << 5); // 设置为pwm模式
//     TIM4->CCMR1 |= (0x1 << 3); // 预加载使能
//     TIM4->CCMR1 &= (~(0x3 << 0)); // 输出模式
//     //  通道使能寄存器
//     TIM4->CCER &= (~(0x1 << 1));
//     TIM4->CCER |= (0x1 << 0); // 设置使能
//     // 配置计算器
//     TIM4->CR1 |= (0x1 << 7);
//     TIM4->CR1 &= (~(0x3 << 5));
//     TIM4->CR1 |= (0x1 << 4);
//     TIM4->CR1 |= (0x1 << 0);
// }




/**
 * @brief   蜂鸣器初始化
 * @param   无
 * @return  无
 */
void hal_tim4_beep_init()
{

	//GPIOB使能
	RCC->MP_AHB4ENSETR |= (0x1 << 1);
	//TIM4EN使能
	RCC->MP_APB1ENSETR |= (0x1 << 2);

	//设置PB6引脚为复用模式
	GPIOB->MODER &= (~(0x3 <<12));
	GPIOB->MODER |= (0x1 << 13);
	//设置PB6引脚功能为AF2--->TIM4_CH1
	GPIOB->AFRL &= (~(0xf << 24));
	GPIOB->AFRL |= (0x1 << 25);


    // 配置TIM4_PSC寄存器
    TIM4->PSC &= (~(0xffff << 0));
    TIM4->PSC |= (0xD0 << 0);

    // 配置TIM4_ARR寄存器——影响周期和频率
    TIM4->ARR &= (~(0xffff << 0));
    TIM4->ARR |= (0x3E8 << 0);

    // 配置TIM4_CCR1寄存器
    TIM4->CCR1 &= (~(0xffff << 0));
    TIM4->CCR1 |= (0x12C << 0);

    // 配置TIM4_CCMR1寄存器
    // 1.设置为pwm模式
    TIM4->CCMR1 &= (~(0x1 << 16));
    TIM4->CCMR1 &= (~(0x1 << 4));
    TIM4->CCMR1 |= (0x3 << 5);
    // 2.设置预加载使能
    TIM4->CCMR1 |= (0x1 << 3);
    // 3.设置为输出模式
    TIM4->CCMR1 &= (~(0x3 << 0));

    // 配置TIM4_CCER寄存器
    // 1.配置起始状态高电平
    TIM4->CCER &= (~(0x1 << 1));
    // 2.配置输出使能
    TIM4->CCER |= (0x1 << 0);

    // 配置tim计算器方式--TIM4_CR1
    // 1.自动重载计数器预加载使能
    TIM4->CR1 |= (0x1 << 7);
    // 2.设置边沿对齐模式
    TIM4->CR1 &= (~(0x3 << 5));
    // 3.设置递减计数器
    TIM4->CR1 |= (0x1 << 4);
    // 4.设置计数器使能
    TIM4->CR1 |= (0x1 << 0);
}

/**
 * @brief   风扇初始化
 * @param   无
 * @return  无
 */
void hal_tim1_fan_init()
{
    /* RCC */
    RCC->MP_AHB4ENSETR |= (0x1 << 4); // GPIOE = AHB4[4]使能
    RCC->MP_APB2ENSETR |= (0x1 << 0); // TIM1_CH1 = APB2[0]使能

    /* GPIO */
    GPIOE->MODER &= (~(0x3)); // 清零
    GPIOE->MODER |= (0x2); // PE9设置复用模式
    GPIOE->AFRH &= (~(0xf << 4)); // 清零
    GPIOE->AFRH |= (0x1 << 4); // PE9设置 TIM1_CH1复用 AF1
}

/**
 * @brief   马达初始化
 * @param
 * @return
 */
void hal_tim16_monitor_init()
{
    /* RCC */
    RCC->MP_AHB4ENSETR |= (0x1 << 5); // PF6 ---> AHB4[5]使能
    RCC->MP_APB2ENSETR |= (0x1 << 3); // TIM16_CH1 ---> APB2[3]使能

    /* GPIO */
    GPIOF->MODER &= (~(0x3 << 12)); // 清零
    GPIOF->MODER |= (0x2 << 12); // PF6 设置端口复用
    GPIOF->AFRL &= (~(0xf << 24)); // 清零
    GPIOF->AFRL |= (0x1 << 24); // PF6 设置 TIM16_CH1复用 AF1
}