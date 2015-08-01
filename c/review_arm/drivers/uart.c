/*
 * uart.c
 *
 *  Created on: 2014-9-21
 *      Author: Administrator
 */
#include"uart.h"
#include "string.h"
void init_uart0()
{
	GPA0.GPA0CON = 0x22;

	UART0.ULCON0 = (0 << 6) | (0 << 3) | (0 << 2) | 0x3 ;

	UART0.UFCON0 = 0;
	UART0.UMCON0 = 0;
	//115200²¨ÌØÂÊ
	UART0.UBRDIV0 = 36 - 1;
	UART0.UDIVSLOT0 = 0x0080;

	UART0.UCON0 = (0 << 10) | (0x1 << 2) | (0x1 << 0) ;

}
void enable_uart_exint()
{
		UART0.UINTP0 = UART0.UINTP0;
		VIC1VECADDR10 = (unsigned int)uart_irq_handler;

		UART0.UINTM0 = ~1;
		VIC1INTERRUPT.VIC1INTENABLE = (1 << 10);
		//send_string("\r\n uart console #");
}
unsigned char recv_one_byte(void)
{
	unsigned char tmp;
	while(0 == (UART0.UTRSTAT0 & 0x1));
	tmp = UART0.URXH0;

	return tmp;
}


void send_one_byte(unsigned char sByte)
{
	while(0 == (UART0.UTRSTAT0 & 0x4));
	UART0.UTXH0 = sByte;
}

void send_string(unsigned char *s)
{
	while(*s != '\0') {
		send_one_byte(*s);
		s++;
	}
}


unsigned char buf[1024];
static int index = 0;
void uart_irq_handler(void)
{
	buf[index] = UART0.URXH0;
	switch(buf[index]) {
		case '\r':
			buf[index] = '\0';
			parse_cmd(buf);
			index = 0;
			my_bzero(buf,sizeof(buf));
			send_string("\r\nuart console #");
			break;
		case 0x8:
			buf[index] = 0;
			if(index > 0)
				index--;
			send_string("\b \b");
			break;
		default:
			send_one_byte(buf[index]);
			index++;
			break;
	}
	UART0.UINTP0 = 1;
}


