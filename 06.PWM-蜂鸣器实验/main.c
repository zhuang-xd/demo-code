#include "pwm.h"

extern void printf(const char *fmt, ...);

int main()
{
    hal_tim4_beep_init();

    while (1) {
        ;
    }

    return 0;
}
