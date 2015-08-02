#include "s5pc100.h"
#include "led.h"
#include "beep.h"
#include "uart.h"
#include "rtc.h"
#include "string.h"

//定义了一个变量，这个变量是一个指针， 指向一个函数
void (*pfunc)(void) ;
void asm_do_irq(void)
{
	//读的是主中断源的处理函数
	pfunc = (void*)VIC0ADDRESS ;
	pfunc () ;

	//清除VIC状态
	VIC0ADDRESS = 0 ;
	VIC1ADDRESS = 0 ;
	VIC2ADDRESS = 0 ;
}

/*
 * AD中断处理函数
 */
unsigned int flag = 0;
unsigned int adval = 0 ;
void adc_irq_handler (void)
{
	adval = ADC.ADCDAT0 & 0xfff;
	//printf ("irq adval = 0x%02x \r\n", adval);
	flag = 1;

	//清除中断标志
	ADC.ADCCLRINT = 1;
}

/*
 * 主函数， 该程序主要测试中断系统的基本初始化
 * 使用UART RX中断实现CONSOLE程序，并完成了 LED ON, LED OFF, BEEP ON,BEEP OFF的功能
 */
int main (void)
{
	led_init ();
	beep_init() ;
	init_uart0 ();

	VIC2VECTADDR.VIC2VECTADDR23 = (unsigned int)adc_irq_handler;
	//通道0
	ADC.ADCMUX = 0 ;
				  //12BIT      预分频              分频值             读转换模式
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


