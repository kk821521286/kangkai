#include "key.h"

void init_key (unsigned int pfunc)
{
	/*======================================================================*/
	//PAD�½ڵļĴ���
	//STEP 1 , ����GPH0_1 ΪEXT1 ����
	GPH0.GPH0CON = (GPH0.GPH0CON & (~(0xf<<4))) | (2 << 4) ;

	//STEP2 : ����EXT1����, ���ü���½���
	WKUP_INT0_7_CON = (WKUP_INT0_7_CON & (~(0x7<<4))) | (2 << 4) ;

	//STEP3 : ��MASK���أ�MASK��
	WKUP_INT0_7_MASK = WKUP_INT0_7_MASK &  (~(1<<1)) ;

	/*=======================================================================*/
	//STEP 4:vic �½ڣ� ʹ�����ж�
	VIC0VECADDR1 = pfunc ; //EXT1
	VIC0INTERRUPT.VIC0INTENABLE = VIC0INTERRUPT.VIC0INTENABLE | (1 << 1) ;
}
