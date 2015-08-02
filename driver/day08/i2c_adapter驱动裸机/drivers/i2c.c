#include "i2c.h"

void i2c_init (void)
{
	GPD.GPDCON = (0X22 << 12) ;
	//ACK      SCL ����ʱ��
	I2C0.I2CCON0 = (1 << 7) | (1 << 6) | (1 << 5) | 0x3;
}

void master_transmit (unsigned short slaveaddr, char *data, unsigned short len)
{

	I2C0.I2CDS0 = slaveaddr ;
	I2C0.I2CSTAT0 = 0xf0 ;		//������ʼ�źţ��ӻ���ַ���ͳ�ȥ

	while ( (I2C0.I2CCON0 & (1 << 4)) == 0 );  //�ȴ��������

	while (len > 0) {
		I2C0.I2CDS0 = *data ;
		I2C0.I2CCON0 = I2C0.I2CCON0 & (~(1 << 4)) ; //PNDλ�������·���һ��������
		while ( (I2C0.I2CCON0 & (1 << 4)) == 0 );  //�ȴ��������
		data ++;
		len -- ;
	}

	I2C0.I2CSTAT0 = 0xd0 ;		//����ֹͣ�ź�
	I2C0.I2CCON0 = I2C0.I2CCON0 & (~(1 << 4)) ;
}

/*
 *
 */
void master_recver (unsigned short slaveaddr, char *data, unsigned short len)
{
	//ACK      SCL ����ʱ��
//	I2C0.I2CCON0 = (1 << 7) | (1 << 6) ;
	I2C0.I2CDS0 = slaveaddr ;
	I2C0.I2CSTAT0 = 0xb0 ;		//������ʼ�źţ��ӻ���ַ���ͳ�ȥ

	while ( (I2C0.I2CCON0 & (1 << 4)) == 0 );  //�ȴ��������

	while (len > 0) {
		I2C0.I2CCON0 = I2C0.I2CCON0 & (~(1 << 4)) ; //PNDλ��������һ�ν���
		while ( (I2C0.I2CCON0 & (1 << 4)) == 0 );   //�������
		*data = I2C0.I2CDS0 ;
		data ++;
		len -- ;
	}

	I2C0.I2CSTAT0 = 0x90 ;		//����ֹͣ�ź�
	I2C0.I2CCON0 = I2C0.I2CCON0 & (~(1 << 4)) ;
}

void master_xfer (struct i2c_msg *msg)
{
	switch (msg->flags) {
	case I2C_M_WR:
		master_transmit (msg->addr, msg->buf, msg->len) ;
		break;
	case I2C_M_RD:
		master_recver (msg->addr, msg->buf, msg->len) ;
		break;

	}

}
