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

void init_rtc(struct rtc_set *set)
{
	RTCCON = RTCCON | 1;

	//对RTC时间进行设置
	RTCBCD.BCDYEAR = set->year ;
	RTCBCD.BCDMON  = set->mon ;
	RTCBCD.BCDDATE = set->data ;
	RTCBCD.BCDHOUR = set->hour ;
	RTCBCD.BCDMIN  = set->min ;
	RTCBCD.BCDSEC  = set->sec ;

	//定时计数初值
	TICCNT = 32768 ;

	//RTC控制 ，使能TICK
	RTCCON = (RTCCON & (~1)) | (1 << 8);
}

void rtc_irq_handler (void)
{
	/*
	send_one_byte ((RTCBCD.BCDYEAR >> 4) + '0');
	send_one_byte ((RTCBCD.BCDYEAR & 0xf) + '0');
	send_one_byte ('-') ;
	send_one_byte ((RTCBCD.BCDMON >> 4) + '0');
	send_one_byte ((RTCBCD.BCDMON & 0xf) + '0');
	send_one_byte ('-') ;
	send_one_byte ((RTCBCD.BCDDATE >> 4) + '0');
	send_one_byte ((RTCBCD.BCDDATE & 0xf) + '0');
	send_one_byte (' ') ;
	send_one_byte ((RTCBCD.BCDHOUR >> 4) + '0');
	send_one_byte ((RTCBCD.BCDHOUR & 0xf) + '0');
	send_one_byte (':') ;
	send_one_byte ((RTCBCD.BCDMIN >> 4) + '0');
	send_one_byte ((RTCBCD.BCDMIN & 0xf) + '0');
	send_one_byte (':') ;
	send_one_byte ((RTCBCD.BCDSEC >> 4) + '0');
	send_one_byte ((RTCBCD.BCDSEC & 0xf) + '0');
	send_string ("\r\n");
	*/
	printf ("%x-%x-%x  %x:%x:%x\r\n",RTCBCD.BCDYEAR, RTCBCD.BCDMON, RTCBCD.BCDDATE, RTCBCD.BCDHOUR,
			RTCBCD.BCDMIN, RTCBCD.BCDSEC) ;
	//清除中断标志位 PND
	RTCINTP = 1 ;
}
/*
 * 主函数， 该程序主要测试中断系统的基本初始化
 * 使用UART RX中断实现CONSOLE程序，并完成了 LED ON, LED OFF, BEEP ON,BEEP OFF的功能
 */
int main (void)
{
	unsigned int adres = 0 ;
	unsigned int val ;
	led_init ();
	beep_init() ;
	init_uart0 ();

	//通道0
	ADC.ADCMUX = 0 ;
				  //12BIT      预分频              分频值             读转换模式
	ADC.ADCCON = (1 << 16) | (1 << 14) | (65 << 6) | (1 << 1);
	adres = ADC.ADCDAT0 ;
	while (1){
		while (!(ADC.ADCCON & (1 << 15))) ;
		adres = ADC.ADCDAT0 & 0xfff ;
		//0-4095, folat vfpv3
		val = adres * 3.3 * 1000 / 4096 ;
		printf ("vol = %d mv\r\n", val);
	}
	return 0 ;
}


