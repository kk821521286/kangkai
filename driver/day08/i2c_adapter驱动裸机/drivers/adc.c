#include "adc.h"


unsigned int adc_conver (int channel)
{
	unsigned int adres = 0 ;
	unsigned int val ;

	ADC.ADCMUX = channel ;
				  //12BIT      Ԥ��Ƶ              ��Ƶֵ             ��ת��ģʽ
	ADC.ADCCON = (1 << 16) | (1 << 14) | (65 << 6) | (1 << 1);
	adres = ADC.ADCDAT0 ;
	while (!(ADC.ADCCON & (1 << 15))) ;
	adres = ADC.ADCDAT0 & 0xfff ;

	return adres ;
}
