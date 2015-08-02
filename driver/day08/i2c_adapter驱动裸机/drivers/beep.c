#include "beep.h"

#define CLK_TIMER 1000000
void beep_init (unsigned int fre)
{
	GPD.GPDCON = (GPD.GPDCON & (~(0xf << 4))) | (2 << 4) ; //¶Á¸ÄÐ´
	//step1 prescale
	TIMER.TCFG0 =  65 ;
	//step2 mux
	TIMER.TCFG1 = TIMER.TCFG1 & (~(0xf << 4)) ;
	//step3 count
	TIMER1.TCNTB1 = CLK_TIMER / fre ;
	//step4 cmp
	TIMER1.TCMPB1 = CLK_TIMER / fre / 2;
	//
	TIMER.TCON = (1 << 9) ;
	//step6
}

void set_beep_frequence (unsigned int fre)
{
	//step3 count
	TIMER1.TCNTB1 = CLK_TIMER / fre ;
	//step4 cmp
	TIMER1.TCMPB1 = CLK_TIMER / fre / 2;
}

void beep_on (void)
{
	TIMER.TCON = (1 << 8) | (1 << 11) ;
}

void beep_off (void)
{
	TIMER.TCON = (0) ;
}
