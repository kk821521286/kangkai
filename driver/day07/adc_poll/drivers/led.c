#include "led.h"

void led_init (void)
{
	GPG3.GPG3CON = 0X1111 ;
}

void led_on (void)
{
	GPG3.GPG3DAT = 0xf ;
}

void led_off(void)
{
	GPG3.GPG3DAT = 0x0 ;
}
