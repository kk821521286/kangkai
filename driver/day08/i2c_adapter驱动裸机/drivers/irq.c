#include "irq.h"

//������һ�����������������һ��ָ�룬 ָ��һ������
static void (*pfunc)(void) ;
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

/*
 * ����һ��ר�ú����� �����жϺ�ʹ���ж�
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

