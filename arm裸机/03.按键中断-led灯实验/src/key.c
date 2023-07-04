#include "key.h"

#define EXTI7_INPUT_EVENT 7
#define EXTI8_INPUT_EVENT 8
#define EXTI9_INPUT_EVENT 9
#define GPIOF_GROUP 0x05
#define KEY1_ID 99
#define KEY2_ID 97
#define KEY3_ID 98
#define PRIORITY_EXTI7 1
#define PRIORITY_EXTI8 2
#define PRIORITY_EXTI9 3

static void hal_exti_init(uint32_t event_id, uint32_t gpio_group, desedge_t mode);
static void hal_gic_init(uint32_t interrupt_id, uint32_t gicd_pritoity);

/**
 * @brief    按键初始化
 * @param    无
 * @return   无
 */
void key_init()
{
    RCC->MP_AHB4ENSETR |= (0x1 << 5); // RCC使能

    // key1 --> PF9
    GPIOF->MODER &= (~(0x3 << 18)); // 00 输入模式
    hal_exti_init(EXTI9_INPUT_EVENT, GPIOF_GROUP, ENABLED);
    hal_gic_init(99, PRIORITY_EXTI9);

    // key2 --> PF7
    GPIOF->MODER &= (~(0x3 << 14)); // 00 输入模式
    hal_exti_init(EXTI7_INPUT_EVENT, GPIOF_GROUP, ENABLED);
    hal_gic_init(97, PRIORITY_EXTI7);

    // key3 --> PF8
    GPIOF->MODER &= (~(0x3 << 16)); // 00 输入模式
    hal_exti_init(EXTI8_INPUT_EVENT, GPIOF_GROUP, ENABLED);
    hal_gic_init(98, PRIORITY_EXTI7);
}


/**
 * @brief    gpiof的exti初始化
 * @param    event_id 事件编号
 * @param    gpio_group GPIOF组的编号
 * @param    mode    触发方式
 * @return   无
 */
static void hal_exti_init(uint32_t event_id, uint32_t gpio_group, desedge_t desedge_mode)
{
    // 1. IRC外部中断选择寄存器
    uint32_t nn = event_id / 4 + 1;
    uint32_t offset = event_id % 4 * 8;

    switch (nn) {
    case 4:
        EXTI->EXTICR4 &= (~(0xff << offset));
        EXTI->EXTICR4 |= (gpio_group << offset);
        break;
    case 3:
        EXTI->EXTICR3 &= (~(0xff << offset));
        EXTI->EXTICR3 |= (gpio_group << offset);
        break;
    case 2:
        EXTI->EXTICR2 &= (~(0xff << offset));
        EXTI->EXTICR2 |= (gpio_group << offset);
        break;
    case 1:
        EXTI->EXTICR1 &= (~(0xff << offset));
        EXTI->EXTICR1 |= (gpio_group << offset);
        break;
    }

    //  2. 下降沿选择寄存器
    EXTI->FTSR1 |= (desedge_mode << event_id); // key enabled

    //  3. 中断屏蔽寄存器
    EXTI->C1IMR1 |= (desedge_mode << event_id); // key unmasked
}

/**
 * @brief    gic初始化
 * @param    interrupt_id 中断号
 * @param    gicd_pritoity 优先级
 * @return   无
 */
static void hal_gic_init(uint32_t interrupt_id, uint32_t gicd_pritoity)
{
    /* GICD */
    uint32_t nn;        // 寄存器编号
    uint32_t offset;    // 寄存器的位偏移量

    // 1. 全局控制寄存器
    GICD->CTRL |= (0x1 << 0); // cpu0 enabled

    // 2. 中断使能控制器 9个
    nn = interrupt_id / 32;
    offset = interrupt_id % 32;
    GICD->ISENABLER[nn] |= (0x1 << offset); // key1

    // 3. 优先级寄存器  72个
    nn = interrupt_id / 4;
    offset = interrupt_id % 4 * 8 + 3;
    GICD->IPRIORITYR[nn] &= (~(0x1f << offset)); 
    GICD->IPRIORITYR[nn] |= (gicd_pritoity << offset); // key1 288 / 72 = 4 ==> 99 / 4 = 24 ... 3

    // 4. 目标分配寄存器 72个
    nn = interrupt_id / 4;
    offset = interrupt_id % 4 * 8;
    GICD->ITARGETSR[nn] &= (~(0x3 << offset));
    GICD->ITARGETSR[nn] |= (0x1 << offset);

    /* GICC */

    // 1. 全局控制寄存器
    GICC->CTRL |= (0x1 << 0); // cpu0

    // 2. 优先级寄存器 (优先级低于GICD)
    GICC->PMR &= (~(0x1f << 3));
    GICC->PMR |= (0x1f << 3);
}