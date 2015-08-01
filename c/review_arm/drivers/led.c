/*
 * led.c
 *
 *  Created on: 2014-9-20
 *      Author: Administrator
 */
#include "led.h"

void init_led()
{
	GPG3.GPG3CON = 0x1111;
}

void led_delay()
{
	int i = 1000000;
	for(;i > 0; i--);
}
void led_on()
{
	GPG3.GPG3DAT = 0xf;
}
void led_off()
{
	GPG3.GPG3DAT = 0x0;
}
