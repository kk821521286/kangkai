#include "i2c.h"

void i2c_init (void)
{
	GPD.GPDCON = (0X22 << 12) ;
	//ACK      SCL 设置时钟
	I2C0.I2CCON0 = (1 << 7) | (1 << 6) | (1 << 5) | 0x3;
}

void master_transmit (unsigned short slaveaddr, char *data, unsigned short len)
{

	I2C0.I2CDS0 = slaveaddr ;
	I2C0.I2CSTAT0 = 0xf0 ;		//发送起始信号，从机地址发送出去

	while ( (I2C0.I2CCON0 & (1 << 4)) == 0 );  //等待发送完成

	while (len > 0) {
		I2C0.I2CDS0 = *data ;
		I2C0.I2CCON0 = I2C0.I2CCON0 & (~(1 << 4)) ; //PND位清零重新发送一个新数据
		while ( (I2C0.I2CCON0 & (1 << 4)) == 0 );  //等待发送完成
		data ++;
		len -- ;
	}

	I2C0.I2CSTAT0 = 0xd0 ;		//发送停止信号
	I2C0.I2CCON0 = I2C0.I2CCON0 & (~(1 << 4)) ;
}

/*
 *
 */
void master_recver (unsigned short slaveaddr, char *data, unsigned short len)
{
	//ACK      SCL 设置时钟
//	I2C0.I2CCON0 = (1 << 7) | (1 << 6) ;
	I2C0.I2CDS0 = slaveaddr ;
	I2C0.I2CSTAT0 = 0xb0 ;		//发送起始信号，从机地址发送出去

	while ( (I2C0.I2CCON0 & (1 << 4)) == 0 );  //等待发送完成

	while (len > 0) {
		I2C0.I2CCON0 = I2C0.I2CCON0 & (~(1 << 4)) ; //PND位清零启动一次接收
		while ( (I2C0.I2CCON0 & (1 << 4)) == 0 );   //接收完成
		*data = I2C0.I2CDS0 ;
		data ++;
		len -- ;
	}

	I2C0.I2CSTAT0 = 0x90 ;		//发送停止信号
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
