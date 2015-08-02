#include "led.h"
#include "string.h"
#include "uart.h"
#include"rtc.h"

struct rtc_set set = {
		0x14,0x09,0x21,0x00,0x00
};
int main(void)
{
	init_led();
	//init_key();
	init_uart0();
	send_string("\r\n start program ");
	enable_uart_exint();
	init_rtc(&set);
	start_timer_tick((unsigned int)rtc_irq_handler);
	while(1) {

	}
	return 0;
}
