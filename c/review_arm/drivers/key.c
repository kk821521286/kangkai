#include"key.h"


void init_key()
{
	// �ⲿ�ж�1 ��EXT1����Ӧ�Ĵ�����
		VIC0VECADDR1 = (unsigned int)key1_handler ; //EXT1
		VIC0VECADDR2 = (unsigned int)key2_handler ; //EXT2
		//VIC0VECADDR3 = (unsigned int)key3_handler ; //EXT3

		//STEP 1 , ����GPH0_1 ΪEXT1 ����
		GPH0.GPH0CON = (GPH0.GPH0CON & (~(0xf<<4))) | (2 << 4) ;
		GPH0.GPH0CON = (GPH0.GPH0CON & (~(0xf<<8))) | (2 << 8) ;  //GPH0_2 EXT2
		//GPH0.GPH0CON = (GPH0.GPH0CON & (~(0xf<<8))) | (2 << 12) ;
		//STEP2 : ����EXT1����, ���ü���½���
		WKUP_INT0_7_CON = (WKUP_INT0_7_CON & (~(0x7<<4))) | (2 << 4) ;
		WKUP_INT0_7_CON = (WKUP_INT0_7_CON & (~(0x7<<8))) | (3 << 8) ;  //ext2 ����Ϊ�����ش���IRQ
		//WKUP_INT0_7_CON = (WKUP_INT0_7_CON & (~(0x7<<12))) | (4 << 12) ;
		//STEP3 : ��MASK���أ�MASK��
		WKUP_INT0_7_MASK = WKUP_INT0_7_MASK &  (~(1<<1)) ;
		WKUP_INT0_7_MASK = WKUP_INT0_7_MASK &  (~(1<<2)) ;   //EXT2 MASK
		//WKUP_INT0_7_MASK = WKUP_INT0_7_MASK &  (~(1<<3)) ;
		//STEP 4:vic �½ڣ� ʹ�����ж�
		VIC0INTERRUPT.VIC0INTENABLE = VIC0INTERRUPT.VIC0INTENABLE | (1 << 1) ;
		VIC0INTERRUPT.VIC0INTENABLE = VIC0INTERRUPT.VIC0INTENABLE | (1 << 2) ; //ETX2 ENABLE
		//VIC0INTERRUPT.VIC0INTENABLE = VIC0INTERRUPT.VIC0INTENABLE | (1 << 3) ;
}
void key1_handler (void)
{
	/*
	 * ����û����жϹ��ܴ���
	 *
	 */
	//led_on ;

	led_on();
	//��PND�Ĵ���,���EXT1 PNDλ
	WKUP_INT0_7_PEND = (1 << 1) ;
}


void key2_handler (void)
{
	//led_off
	led_off();
	//��PND�Ĵ���,���EXT1 PNDλ
	WKUP_INT0_7_PEND = (1 << 2) ;
}
/*
void key3_handler (void)
{
	//led_off
	led_off();
	//��PND�Ĵ���,���EXT1 PNDλ
	WKUP_INT0_7_PEND = (1 << 3) ;
}

*/
