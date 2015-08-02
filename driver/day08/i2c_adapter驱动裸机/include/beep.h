#ifndef __BEEP_H
#define __BEEP_H

#include "s5pc100.h"

void beep_init (unsigned int fre);
void set_beep_frequence (unsigned int fre);
void beep_on (void);
void beep_off(void);

#endif
