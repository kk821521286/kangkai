//#include"spi.h"
//
//void SPIx_Init(void)
//{
//	RCC->APB2ENR|=1<<2;		   //使能PORTA时钟
//	RCC->APB2ENR|=1<<12;	   //SPI1时钟使能
//
//	GPIOA->CRL&=0X000FFFFF;	   //清除该位原来的设置
//	GPIOA->CRL|=0XBBB00000;	   //设置PA567为复用推挽输出
//	GPIOA->ODR|=0X7<<5;		   //上拉
//
//	SPI1->CR1|=0<<10;  //设置SPI总线为全双工模式
//	SPI1->CR1|=1<<9;   //启用软件NSS管理
//	SPI1->CR1|=1<<8;   //该位只有在SSM为1时有意义，它决定了NSS上的电平，在NSS引脚上的IO操作无效
//	SPI1->CR1|=1<<2;   //设置SPI为主设备
//	SPI1->CR1|=0<<11;  //8位数据传输格式
//	SPI1->CR1|=1<<1;   //在空闲模式下SCK保持为高电平
//	SPI1->CR1|=1<<0;   //CHPA=1；数据采样从第二个边沿开始
//	SPI1->CR1|=7<<3;   //波特率
//	SPI1->CR1|=0<<7;   //MSB在前
//	SPI1->CR1|=1<<6;   //SPI设备使能 
//
//	SPIx_ReadWriteByte(0xff);	//启动传输
//
//}
//
//void SPIx_SetSpeed(u8 SpeedSet)
//{
//	SPI1->CR1&=0xFFC7;
//	if(SpeedSet==SPI_SPEED_2)
//	{
//		SPI1->CR1=0<<3;
//	}
//	else if(SpeedSet==SPI_SPEED_8)
//	{
//		SPI1->CR1=2<<3;
//	}
//	else if(SpeedSet==SPI_SPEED_16)
//	{
//		SPI1->CR1=3<<3;
//	}
//	else
//	{
//		SPI1->CR1=7<<3;
//	}
//	SPI1->CR1|=1<<8;   //SPI设备使能
//}
//
//u8 SPIx_ReadWriteByte(u8 TxData) 
//{
//	u8 retry=0;
//	while((SPI1->SR&1<<1)==0)	 //等待发送区为空  0：发送缓冲为非空  1：发送缓冲为空
//	{
//		retry++;				 //发送缓冲区若为非空（即还未发送完毕），等待发送
//		if(retry>200)
//		return 0;
//	}
//	SPI1->DR=TXData;			 //发送一个字节 
//	retry=0;
//	while((SPI1->SR&1<<0)==0)	 //等待接收一个字节
//	{
//		retry++;
//		if(retry>200)
//		return 0;
//	}
//	return SPI1->DR;			 //返回收到的数据
//}
//
//
//
//
//
//
