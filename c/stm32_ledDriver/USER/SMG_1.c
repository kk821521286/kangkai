#include<stm32f10x_lib.h>
#include"sys.h"
#include"delay.h"
#include"usart.h"
#include"74h595.h"

u8 seg_table[]={
0x3f,0x06,0x5b,0x4f,
0x66,0x6d,0x7d,0x07,
0x7f,0x6f,0x77,0x7c,
0x39,0x5e,0x79,0x71};

int main(void) 
{
	u8 i;
	Stm32_Clock_Init(9);  //系统时钟设置
	delay_init(72);		  //延时初始化
	H595_Init();

	while(1)
	{
		A138=1;B138=0;C138=0;
		for(i=0;i<16;i++)		
		{
			write_smg(seg_table[i]);
			delay_ms(500);
		}
	}
	
}














