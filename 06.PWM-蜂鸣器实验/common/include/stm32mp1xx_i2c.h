#ifndef __STM32MP1xx_I2C_H__
#define __STM32MP1xx_I2C_H__

typedef struct{
	volatile unsigned int CR1;
	volatile unsigned int CR2;
	volatile unsigned int OAR1;
	volatile unsigned int OAR2;
	volatile unsigned int TIMINFR;
	volatile unsigned int TIMEOUTR;
	volatile unsigned int ISR;
	volatile unsigned int ICR;
	volatile unsigned int PECR;
	volatile unsigned int RXDR;
	volatile unsigned int TXDR;

}i2c_t;

#define I2C1   ((i2c_t *)0x40012000)   // APB1
#define I2C2   ((i2c_t *)0x40013000)   // APB1
#define I2C3   ((i2c_t *)0x40014000)   // APB1
#define I2C4   ((i2c_t *)0x5C002000)   // APB5
#define I2C5   ((i2c_t *)0x40015000)   // APB1
#define IIC6   ((i2c_t *)0x5C009000)   // APB5



#endif // __STM32MP1xx_I2C_H__
