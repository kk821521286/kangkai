#include "rtc.h"

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
	//RTC控制 ，使能TICK
	RTCCON = (RTCCON & (~1)) ;
}

void get_rtc(struct rtc_set *set)
{
	//对RTC时间进行设置
	set->year = RTCBCD.BCDYEAR ;
	set->mon  = RTCBCD.BCDMON  ;
	set->data = RTCBCD.BCDDATE ;
	set->hour = RTCBCD.BCDHOUR ;
	set->min  = RTCBCD.BCDMIN  ;
	set->sec  = RTCBCD.BCDSEC  ;
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

void stop_timer_tick (void)
{
	RTCCON = RTCCON & (~(1 << 8));
}

/*
 * 闹铃初始化函数
 */
void set_alarm (struct rtc_set *set)
{
	//对RTC时间进行设置
	RTALM.ALMYEAR = set->year ;
	RTALM.ALMMON  = set->mon ;
	RTALM.ALMDATE = set->data ;
	RTALM.ALMHOUR = set->hour ;
	RTALM.ALMMIN  = set->min ;
	RTALM.ALMSEC  = set->sec ;

}

//pfunc是中断处理函数的入口地址
void start_alarm (unsigned int val, unsigned int pfunc)
{
	RTALM.RTCALM = val ;
	if (pfunc != 0) {
		VIC0VECADDR28 = pfunc ;
	}
}

void stop_alarm (void)
{
	RTALM.RTCALM = 0 ;
}



