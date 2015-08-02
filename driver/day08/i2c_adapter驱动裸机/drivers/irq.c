#include "irq.h"

//定义了一个变量，这个变量是一个指针， 指向一个函数
static void (*pfunc)(void) ;
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

/*
 * 定义一个专用函数， 根据中断号使能中断
 */
void asm_enable_irq(unsigned int irqnumber)
{
	if (irqnumber >= 96)
		return ;

	if (irqnumber < 32) {
		VIC0INTERRUPT.VIC0INTENABLE = VIC0INTERRUPT.VIC0INTENABLE | (1 << irqnumber) ;
	} else if((irqnumber >= 32) && (irqnumber < 64)) {
		irqnumber -= 32 ;
		VIC1INTERRUPT.VIC1INTENABLE = VIC1INTERRUPT.VIC1INTENABLE | (1 << irqnumber) ;
	} else if((irqnumber >= 64) && (irqnumber < 96)) {
		irqnumber -= 64 ;
		VIC2INTERRUPT.VIC2INTENABLE = VIC2INTERRUPT.VIC2INTENABLE | (1 << irqnumber) ;
	}
}

