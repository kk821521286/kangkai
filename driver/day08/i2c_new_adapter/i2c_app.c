#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

unsigned char point ;
struct i2c_msg msg = {
	.addr = 0x48,
	.flags = 0,
	.len = 1,
	.buf = &point
};

struct i2c_msgss {
	struct i2c_msg *msgs;
	int num;
} ;
struct i2c_msgss ss;
int main(void)
{
	char *buffer = "hh";
	int fd = open ("/dev/i2c-0", O_RDWR);
	if (fd < 0) {
		perror ("open i2c-0 error");
		exit (0);
	}

	ss.msgs = &msg;
	ss.num = 1;
	ioctl (fd,I2C_RDWR,&ss);
	//write (fd, buffer, 2);

	close (fd);

	return 0;
} 

