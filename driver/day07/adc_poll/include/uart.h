#ifndef _UART_H
#define _UART_H

#include "s5pc100.h"

void init_uart0(void);
unsigned char recv_one_byte (void);
void send_one_byte (unsigned char sByte);
void send_string (unsigned char *s) ;

#endif
