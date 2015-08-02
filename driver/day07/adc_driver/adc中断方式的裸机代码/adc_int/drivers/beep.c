#include "beep.h"

void beep_init (void)
{
	GPD.GPDCON = (GPD.GPDCON & (~(0xf << 4))) | (2 << 4) ; //¶Á¸ÄÐ´
	//step1 prescale
	TIMER.TCFG0 =  65 ;
	//step2 mux
	TIMER.TCFG1 = TIMER.TCFG1 & (~(0xf << 4)) ;
	//step3 count
	TIMER1.TCNTB1 = 10000 ;
	//step4 cmp
	TIMER1.TCMPB1 = 10000 / 2 ;
	//
	TIMER.TCON = (1 << 9) ;
	//step6
}

void beep_on (void)
{
	TIMER.TCON = (1 << 8) | (1 << 11) ;
}

void beep_off (void)
{
	TIMER.TCON = (0) ;
}
