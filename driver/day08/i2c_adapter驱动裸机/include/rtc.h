#ifndef _RTC_H
#define _RTC_H

#include "s5pc100.h"

struct rtc_set {
	unsigned char year ;
	unsigned char mon ;
	unsigned char data;
	unsigned char hour;
	unsigned char min;
	unsigned char sec ;
};

void init_rtc(struct rtc_set *set);
void get_rtc(struct rtc_set *set);

void start_timer_tick (unsigned int pfunc);
void stop_timer_tick (void);

void set_alarm (struct rtc_set *set);
void start_alarm (unsigned int val, unsigned int pfunc);
void stop_alarm (void);

#endif
