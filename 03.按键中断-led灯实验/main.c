#include "key.h"
#include "led.h"

extern void printf(const char *fmt, ...);
void delay_ms(int ms);

/** 
* @brief    入口函数
* @param    无
* @return   无
*/
int main()
{
    led_init();
    key_init();

    while (1){
        printf("running...\n");
        delay_ms(2000);
    }

    return 0;
}

/** 
* @brief    延时函数
* @param    ms  毫秒
* @return   无
*/
void delay_ms(int ms)
{
    int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 1800; j++)
            ;
}