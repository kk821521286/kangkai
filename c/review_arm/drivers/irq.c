/*
 * irq.c
 *
 *  Created on: 2014-9-21
 *      Author: Administrator
 */
#include "irq.h"
void asm_do_irq(void)
{
	//���������ж�Դ�Ĵ�����
	pfunc = (void*)VIC0ADDRESS ;
	pfunc () ;

	//���VIC״̬
	VIC0ADDRESS = 0 ;
	VIC1ADDRESS = 0 ;
	VIC2ADDRESS = 0 ;
}
