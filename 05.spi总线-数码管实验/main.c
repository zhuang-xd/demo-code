#include "nixie_light.h"

extern void printf(const char *fmt, ...);

int main()
{

    SPI_init();
    while (1) {
        // exam1();
        exam2();
    }

    return 0;
}
