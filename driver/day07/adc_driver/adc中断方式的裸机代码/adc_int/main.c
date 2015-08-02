#include "s5pc100.h"
#include "led.h"
#include "beep.h"
#include "uart.h"
#include "rtc.h"
#include "string.h"

//������һ�����������������һ��ָ�룬 ָ��һ������
void (*pfunc)(void) ;
void asm_do_irq(void)
{
	//���������ж�Դ�Ĵ�����
	pfunc = (void*)VIC0ADDRESS ;
	pfunc () ;

	//���VIC״̬
	VIC0ADDRESS = 0 ;
	VIC1ADDRESS = 0 ;
	VIC2ADDRESS = 0 ;
}

/*
 * AD�жϴ�����
 */
unsigned int flag = 0;
unsigned int adval = 0 ;
void adc_irq_handler (void)
{
	adval = ADC.ADCDAT0 & 0xfff;
	//printf ("irq adval = 0x%02x \r\n", adval);
	flag = 1;

	//����жϱ�־
	ADC.ADCCLRINT = 1;
}

/*
 * �������� �ó�����Ҫ�����ж�ϵͳ�Ļ�����ʼ��
 * ʹ��UART RX�ж�ʵ��CONSOLE���򣬲������ LED ON, LED OFF, BEEP ON,BEEP OFF�Ĺ���
 */
int main (void)
{
	led_init ();
	beep_init() ;
	init_uart0 ();

	VIC2VECTADDR.VIC2VECTADDR23 = (unsigned int)adc_irq_handler;
	//ͨ��0
	ADC.ADCMUX = 0 ;
				  //12BIT      Ԥ��Ƶ              ��Ƶֵ             ��ת��ģʽ
	ADC.ADCCON = (1 << 16) | (1 << 14) | (65 << 6) | (0 << 1);

	VIC2INTERRUPT.VIC2INTENABLE = VIC2INTERRUPT.VIC2INTENABLE | (1 << 23);

	while (1){
		ADC.ADCCON = ADC.ADCCON | (1 << 0);
		while (flag == 0);
		flag = 0;
		adval = adval * 3300 / 4096;
		printf ("vol = %d mv\r\n", adval);
	}
	return 0 ;
}


