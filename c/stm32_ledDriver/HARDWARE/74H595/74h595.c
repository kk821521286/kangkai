#include"74h595.h"
#include"delay.h"

void H595_Init(void)
{
	RCC->APB2ENR|=1<<2;
	RCC->APB2ENR|=1<<3;

	GPIOA->CRH&=0XFFFFFFF0;
	GPIOA->CRH|=0X00000003;
	GPIOA->ODR|=1<<8;

	GPIOA->CRL&=0XF000FFFF;
	GPIOA->CRL|=0X03330000;
	GPIOA->ODR|=1<<4;
	GPIOA->ODR|=1<<5;			//�˿���������Ĵ�������þͼĴ���д���ݿ��Կ���IO�������ƽ
	GPIOA->ODR|=1<<6;

	GPIOB->CRL&=0XFFFFF000;
	GPIOB->CRL|=0X00000333;
	GPIOB->ODR|=1<<0;
	GPIOB->ODR|=1<<1;			//�˿���������Ĵ�������þͼĴ���д���ݿ��Կ���IO�������ƽ
	GPIOB->ODR|=1<<2;
}

void write_smg(u8 temp)
{
	u8 i;
	 for(i=0;i<8;i++)
	 {
		  SH=0;
		  if((temp&0x80) == 0x80)						
		  {
			  DS = 1; //�����������
		  }
		  else
		  {
			  DS = 0;
		  } 
		 SH=1; //��λ����ʱ�ӣ�����������
		 temp<<=1;
	     delay_us(3);
		 SH=0;
	 }
	 
	 ST=0; //�������ʱ��
	 delay_us(3);	
	 ST=1;
	 delay_us(3);	
	 ST=0;	
}








