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
	GPIOA->ODR|=1<<5;			//端口数据输出寄存器，向该就寄存器写数据可以控制IO的输出电平
	GPIOA->ODR|=1<<6;

	GPIOB->CRL&=0XFFFFF000;
	GPIOB->CRL|=0X00000333;
	GPIOB->ODR|=1<<0;
	GPIOB->ODR|=1<<1;			//端口数据输出寄存器，向该就寄存器写数据可以控制IO的输出电平
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
			  DS = 1; //串行数据输出
		  }
		  else
		  {
			  DS = 0;
		  } 
		 SH=1; //移位输入时钟，上升沿输入
		 temp<<=1;
	     delay_us(3);
		 SH=0;
	 }
	 
	 ST=0; //并行输出时钟
	 delay_us(3);	
	 ST=1;
	 delay_us(3);	
	 ST=0;	
}








