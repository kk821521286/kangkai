/*
 * i2c.h
 *
 *  Created on: 2014-9-18
 *      Author: Administrator
 */

#ifndef I2C_H_
#define I2C_H_
#include "s5pc100.h"

#define I2C_M_WR        0x0000  /* read data, from slave to master */
#define I2C_M_RD        0x0001  /* read data, from slave to master */
struct i2c_msg {
	unsigned short addr;
	unsigned short flags ;
	unsigned int len ;
	char *buf;
};

void i2c_init(void) ;
void master_transmit (unsigned short slaveaddr, char *data, unsigned short len);
void master_recver (unsigned short slaveaddr, char *data, unsigned short len);
void master_xfer (struct i2c_msg *msg);

#endif /* I2C_H_ */
