/*
 * irq.c
 *
 *  Created on: 2014-9-21
 *      Author: Administrator
 */
#include "irq.h"
void asm_do_irq(void)
{
	//读的是主中断源的处理函数
	pfunc = (void*)VIC0ADDRESS ;
	pfunc () ;

	//清除VIC状态
	VIC0ADDRESS = 0 ;
	VIC1ADDRESS = 0 ;
	VIC2ADDRESS = 0 ;
}
