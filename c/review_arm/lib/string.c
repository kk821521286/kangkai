/*
 * string.c
 *
 *  Created on: 2014-9-21
 *      Author: Administrator
 */
#include "string.h"

void my_bzero (void *s, int len)
{
	char *p = s ;

	while (len > 0) {
		*p ++ = 0 ;
		len -- ;
	}
}

int my_strcmp (const char *src, const char *dest)
{
	while ((*src != '\0') && (*dest != '\0') && (*src == *dest)) {
			src ++ ;
			dest ++ ;
	}

	return *src - *dest ;
}

void parse_cmd (unsigned char *pcmd)
{
	if (my_strcmp ("led on", pcmd) == 0) {
		led_on () ;
	}
	else if (my_strcmp ("led off", pcmd) == 0) {
		led_off ();
	}
	else {

	}
}
