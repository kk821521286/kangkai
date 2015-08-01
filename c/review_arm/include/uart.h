/*
 * uart.h
 *
 *  Created on: 2014-9-21
 *      Author: Administrator
 */

#ifndef UART_H_
#define UART_H_

#include "s5pc100.h"

extern void init_uart0();
extern void enable_uart_exint();
extern unsigned char recv_one_byte(void);
extern void send_one_byte(unsigned char sByte);
extern void send_string(unsigned char *s);
extern void uart_irq_handler(void);
#endif /* UART_H_ */
