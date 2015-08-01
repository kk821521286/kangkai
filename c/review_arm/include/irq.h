/*
 * irq.h
 *
 *  Created on: 2014-9-21
 *      Author: Administrator
 */

#ifndef IRQ_H_
#define IRQ_H_
#include "s5pc100.h"
void (*pfunc)(void) ;
extern void asm_do_irq(void);
#endif /* IRQ_H_ */
