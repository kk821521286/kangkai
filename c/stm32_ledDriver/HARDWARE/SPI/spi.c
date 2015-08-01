//#include"spi.h"
//
//void SPIx_Init(void)
//{
//	RCC->APB2ENR|=1<<2;		   //ʹ��PORTAʱ��
//	RCC->APB2ENR|=1<<12;	   //SPI1ʱ��ʹ��
//
//	GPIOA->CRL&=0X000FFFFF;	   //�����λԭ��������
//	GPIOA->CRL|=0XBBB00000;	   //����PA567Ϊ�����������
//	GPIOA->ODR|=0X7<<5;		   //����
//
//	SPI1->CR1|=0<<10;  //����SPI����Ϊȫ˫��ģʽ
//	SPI1->CR1|=1<<9;   //�������NSS����
//	SPI1->CR1|=1<<8;   //��λֻ����SSMΪ1ʱ�����壬��������NSS�ϵĵ�ƽ����NSS�����ϵ�IO������Ч
//	SPI1->CR1|=1<<2;   //����SPIΪ���豸
//	SPI1->CR1|=0<<11;  //8λ���ݴ����ʽ
//	SPI1->CR1|=1<<1;   //�ڿ���ģʽ��SCK����Ϊ�ߵ�ƽ
//	SPI1->CR1|=1<<0;   //CHPA=1�����ݲ����ӵڶ������ؿ�ʼ
//	SPI1->CR1|=7<<3;   //������
//	SPI1->CR1|=0<<7;   //MSB��ǰ
//	SPI1->CR1|=1<<6;   //SPI�豸ʹ�� 
//
//	SPIx_ReadWriteByte(0xff);	//��������
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
//	SPI1->CR1|=1<<8;   //SPI�豸ʹ��
//}
//
//u8 SPIx_ReadWriteByte(u8 TxData) 
//{
//	u8 retry=0;
//	while((SPI1->SR&1<<1)==0)	 //�ȴ�������Ϊ��  0�����ͻ���Ϊ�ǿ�  1�����ͻ���Ϊ��
//	{
//		retry++;				 //���ͻ�������Ϊ�ǿգ�����δ������ϣ����ȴ�����
//		if(retry>200)
//		return 0;
//	}
//	SPI1->DR=TXData;			 //����һ���ֽ� 
//	retry=0;
//	while((SPI1->SR&1<<0)==0)	 //�ȴ�����һ���ֽ�
//	{
//		retry++;
//		if(retry>200)
//		return 0;
//	}
//	return SPI1->DR;			 //�����յ�������
//}
//
//
//
//
//
//
