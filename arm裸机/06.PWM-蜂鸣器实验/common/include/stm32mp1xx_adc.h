#ifndef __STM32MP1xx_ADC_H__
#define __STM32MP1xx_ADC_H__

typedef struct{
	volatile unsigned int ISR;
	volatile unsigned int IER;
	volatile unsigned int CR;
	volatile unsigned int CFGR;
	volatile unsigned int CFGR2;
	volatile unsigned int SMPR1;
	volatile unsigned int SMPR2;
	volatile unsigned int PCSEL;
	volatile unsigned int LTR1;
	volatile unsigned int HTR1;
	volatile unsigned int RES1[2];
	volatile unsigned int SQR1;
	volatile unsigned int SQR2;
	volatile unsigned int SQR3;
	volatile unsigned int SQR4;
	volatile unsigned int DR;
	volatile unsigned int RES2[2];
	volatile unsigned int JSQR;
	volatile unsigned int RES3[4];
	volatile unsigned int OFR1;
	volatile unsigned int OFR2;
	volatile unsigned int OFR3;
	volatile unsigned int OFR4;
	volatile unsigned int RES4[4];
	volatile unsigned int JDR1;
	volatile unsigned int JDR2;
	volatile unsigned int JDR3;
	volatile unsigned int JDR4;
	volatile unsigned int RES5[4];
	volatile unsigned int AWD2CR;
	volatile unsigned int AWD3CR;
	volatile unsigned int RES6[2];
	volatile unsigned int LTR2;
	volatile unsigned int HTR2;
	volatile unsigned int LTR3;
	volatile unsigned int HTR3;
	volatile unsigned int DIFSEL;
	volatile unsigned int CALFACT;
	volatile unsigned int CALFACT2;
	volatile unsigned int RES7[1];
	volatile unsigned int OR;
}adc_t;

#define  ADC   ((adc_t *)0x48003000)

typedef struct{
	volatile unsigned int CSR;
	volatile unsigned int RES1;
	volatile unsigned int CCR;
	volatile unsigned int CDR;
	volatile unsigned int CDR2;
}adc_common_t;
#define  ADC_COMMON ((adc_common_t *)0x48003300)

#endif // __STM32MP1xx_ADC_H__
