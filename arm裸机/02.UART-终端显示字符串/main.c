#include "led.h"
int main()
{
	all_led_init();
	while(1)
	{
		all_led_flash();
	}
	return 0;
}
