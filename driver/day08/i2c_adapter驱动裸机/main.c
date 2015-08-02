#include "s5pc100.h"
#include "led.h"
#include "beep.h"
#include "uart.h"
#include "rtc.h"
#include "key.h"
#include "i2c.h"
#include "string.h"


/*
 * ARM I2C PROJECT
 */
int main (void)
{
	struct i2c_msg msg;
	unsigned char point = 0 ;
	unsigned char recvbuf [2] ;
	init_uart0() ;
	i2c_init ();
	printf ("lm75 start\r\n") ;

	while (1){
		msg.addr = 0X90 ;
		msg.flags = I2C_M_WR;
		msg.len = 1;
		msg.buf = &point ;
		master_xfer (&msg);

		msg.addr = 0X90 ;
		msg.flags = I2C_M_RD;
		msg.len = 2;
		msg.buf = &recvbuf ;
		master_xfer (&msg);

		printf ("temp = %d\r\n ", recvbuf[0]) ;
	}
	return 0 ;
}


