#ifndef  __STM32MP157_SPI_H__
#define  __STM32MP157_SPI_H__



typedef struct {
	volatile unsigned int CR1;
	volatile unsigned int CR2;
	volatile unsigned int CFG1;
	volatile unsigned int CFG2;
	volatile unsigned int IER;
	volatile unsigned int SR;
	volatile unsigned int IFCR;
	volatile unsigned int TXDR;
	volatile unsigned int RES1[3];
	volatile unsigned int RXDR;
	volatile unsigned int RES2[3];
	volatile unsigned int CRCPOLY;
	volatile unsigned int TXCRC;
	volatile unsigned int RXCRC;
	volatile unsigned int UDRDR;
	volatile unsigned int I2SCFGR;
	volatile unsigned int I2S_HWCFGR;
	volatile unsigned int VERR;
	volatile unsigned int IPIDR;
	volatile unsigned int SIDR;
}spi_t;
#define  SPI1  ((spi_t *)0x44004000)   // APB2
#define  SPI2  ((spi_t *)0x4000B000)   // APB1
#define  SPI3  ((spi_t *)0x4000C000)   // APB1
#define  SPI4  ((spi_t *)0x44005000)   // APB2
#define  SPI5  ((spi_t *)0x44009000)   // APB2

#endif //__STM32MP157_SPI_H__
