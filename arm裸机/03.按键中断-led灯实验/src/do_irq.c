#include "key.h"
#include "led.h"

extern void delay_ms(int ms);
extern void printf(const char *fmt, ...);
unsigned int i = 0;

/**
 * @brief    中断处理函数
 * @param    无
 * @return   无
 * @note     irq触发后打印一句话
 */
void do_irq(void)
{
    // 1. 获取中断号
    unsigned int interrupt_id = GICC->IAR & 0x3FF;

    // 2. 判断中断号
    switch (interrupt_id) {
    case 99: // key1
        delay_ms(500);
        printf("key1 onpressed  ##### interrupt_id = %d ############\n",interrupt_id);
        toggle_led(1);
        // 3. 清除中断挂起标志位（EXTI、GICD）
        EXTI->FPR1 |= (0x1 << 9); // key1 clear bit
        GICD->ICPENDR[3] |= (0x1 << 3); // 288 / 9 = 32 => 99 / 32 = 3 ... 3
        break;
    case 97: // key2
        delay_ms(500);
        printf("key2 onpressed  ##### interrupt_id = %d ############\n",interrupt_id);
        toggle_led(2);
        // 3. 清除中断挂起标志位（EXTI、GICD）
        EXTI->FPR1 |= (0x1 << 7); // key2 clear bit
        GICD->ICPENDR[3] |= (0x1 << 1); // 288 / 9 = 32 => 97 / 32 = 3 ... 1
        break;
    case 98: // key3
        delay_ms(500);
        printf("key3 onpressed  ##### interrupt_id = %d ############\n",interrupt_id);
        toggle_led(3);
        // 3. 清除中断挂起标志位（EXTI、GICD）
        EXTI->FPR1 |= (0x1 << 8); // key3 clear bit
        GICD->ICPENDR[3] |= (0x1 << 2); // 288 / 9 = 32 => 98 / 32 = 3 ... 2
        break;

    default:
        break;
    }

    // 4. 清除中断号（GICC）
    GICC->EOIR = interrupt_id;
}



