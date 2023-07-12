#ifndef _HEAD_H_
#define _HEAD_H_

// 定义寄存器组织结构体
typedef struct {
    unsigned int moder;
    unsigned int otyper;
    unsigned int ospeedr;
    unsigned int pupdr;
    unsigned int idr;
    unsigned int odr;
} gpio_t;

// 物理基地址
#define PHY_RCC        0x50000A28
#define PHY_LED1_ADDR  0x50006000 // PE10
#define PHY_LED2_ADDR  0x50007000 // PF10
#define PHY_LED3_ADDR  0x50006000 // PE8

// GPIO控制
#define LED_ON    _IO('l', 1) // 1开 2关
#define LED_OFF   _IO('l', 2)

#endif //_HEAD_H_