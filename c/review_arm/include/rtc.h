/*
 * rtc.h
 *
 *  Created on: 2014-9-21
 *      Author: Administrator
 */

#ifndef RTC_H_
#define RTC_H_
#include"s5pc100.h"
struct rtc_set {
	unsigned char year ;
	unsigned char mon ;
	unsigned char data;
	unsigned char hour;
	unsigned char min;
	unsigned char sec ;
};
extern void rtc_irq_handler (void);
extern void init_rtc(struct rtc_set *set);

extern void start_timer_tick (unsigned int pfunc);
#endif /* RTC_H_ */
