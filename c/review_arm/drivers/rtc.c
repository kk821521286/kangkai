/*
 * rtc.c
 *
 *  Created on: 2014-9-21
 *      Author: Administrator
 */
#include "rtc.h"


void init_rtc(struct rtc_set *set)
{
	RTCCON = RTCCON | 1;

		RTCBCD.BCDYEAR = set->year ;
		RTCBCD.BCDMON  = set->mon ;
		RTCBCD.BCDDATE = set->data ;
		RTCBCD.BCDHOUR = set->hour ;
		RTCBCD.BCDMIN  = set->min ;
		RTCBCD.BCDSEC  = set->sec ;
	RTCCON = (RTCCON & (~1));
}

void rtc_irq_handler (void)
{

	send_one_byte ((RTCBCD.BCDSEC >> 4) + '0');
	send_one_byte ((RTCBCD.BCDSEC & 0xf) + '0');
	//printf ("[%x-%x-%x  %x:%x:%x] ",RTCBCD.BCDYEAR, RTCBCD.BCDMON, RTCBCD.BCDDATE, RTCBCD.BCDHOUR,
		//	RTCBCD.BCDMIN, RTCBCD.BCDSEC) ;

	//清除中断标志位 PND
	RTCINTP = 1 ;
}

void start_timer_tick (unsigned int pfunc)
{
	RTCCON = RTCCON | 1;

	if (pfunc != 0) {
		VIC0VECADDR29 = pfunc ;
	}
	//定时计数初值
	TICCNT = 32768 ;

	RTCCON = (RTCCON & (~1)) | (1 << 8);
}
