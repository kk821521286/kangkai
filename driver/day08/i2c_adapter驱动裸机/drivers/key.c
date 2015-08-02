#include "key.h"

void init_key (unsigned int pfunc)
{
	/*======================================================================*/
	//PAD章节的寄存器
	//STEP 1 , 设置GPH0_1 为EXT1 功能
	GPH0.GPH0CON = (GPH0.GPH0CON & (~(0xf<<4))) | (2 << 4) ;

	//STEP2 : 设置EXT1功能, 设置监控下降沿
	WKUP_INT0_7_CON = (WKUP_INT0_7_CON & (~(0x7<<4))) | (2 << 4) ;

	//STEP3 : 打开MASK开关（MASK）
	WKUP_INT0_7_MASK = WKUP_INT0_7_MASK &  (~(1<<1)) ;

	/*=======================================================================*/
	//STEP 4:vic 章节， 使能主中断
	VIC0VECADDR1 = pfunc ; //EXT1
	VIC0INTERRUPT.VIC0INTENABLE = VIC0INTERRUPT.VIC0INTENABLE | (1 << 1) ;
}
