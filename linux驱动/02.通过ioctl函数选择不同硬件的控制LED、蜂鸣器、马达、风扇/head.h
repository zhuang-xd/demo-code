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
#define PHY_FAN_ADDR   0x50006000 // PE9
#define PHY_MOTOR_ADDR 0x50007000 // PF6
#define PHY_BEEP_ADDR  0x50003000 // PB6

// GPIO控制
#define LED_ON    _IO('1', 1) // 1开 2关
#define LED_OFF   _IO('1', 2)
#define FAN_ON    _IO('1', 1)
#define FAN_OFF   _IO('1', 2)
#define MOTOR_ON  _IO('1', 1)
#define MOTOR_OFF _IO('1', 2)
#define BEEP_ON   _IO('1', 1)
#define BEEP_OFF  _IO('1', 2)

#define PHY_GPIOE 0X50006000
#define PHY_GPIOF 0X50007000
#endif //_HEAD_H_