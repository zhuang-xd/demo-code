#include "gpio.h"

// gpio初始化函数 
// 函数的参数： 
// 		gpio : 那组的GPIO引脚
// 		pin : 哪个引脚
/*void hal_gpio_init(gpio_t *gpio,unsigned int pin)
{
	// 1. 设置 GPIO引脚为输出模式 
	gpio->MODER &= (~(0x3 << (pin * 2)));
	gpio->MODER |= (0x1 << (pin * 2));
	// 2. 设置 GPIO引脚为推挽输出
	// 3. 设置 GPIO引脚为低速模式 
	// 4. 设置 GPIO引脚禁止上拉和下拉
}

// 函数功能：gpio引脚输出高低电平
// 函数的参数：
// 		gpio : 那组的GPIO引脚
// 		pin : 哪个引脚
// 		state : 引脚的状态
// 函数返回值：无
void hal_gpio_write(gpio_t *gpio, 
		unsigned int pin, gpio_states_t state)
{
	// 5. 设置GPIO引脚输出高低电瓶

}

// 函数功能：gpio 引脚输出的状态
// 函数的参数： 
// 		gpio : 那组的GPIO引脚
// 		pin : 哪个引脚
// 函数的返回值： 
// 		引脚的输入状态
gpio_states_t hal_gpio_read(gpio_t *gpio, 
		unsigned int pin)
{
	return GPIO_RESET;
}
*/
