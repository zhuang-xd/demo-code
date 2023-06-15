#include "si7006.h"

extern void printf(const char *fmt, ...);
void delay_ms(int ms)
{
    int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 1800; j++)
            ;
}

int main()
{
    short temp;
    unsigned short hum;
    si7006_init();
    while (1) {
        hum = si7006_read_hum_data(SI7006_SLAVE, 0xE5);
        temp = si7006_read_temp_data(SI7006_SLAVE, 0xE3);
        printf("hum = %d\n", 125 * hum / 65536 - 6);
        printf("temp = %d\n", 176 * temp / 65536 - 47);
    }
    return 0;
}
