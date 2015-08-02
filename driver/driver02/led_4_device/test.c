#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>

#define CMD_LED_ON  _IO ('k', 0)
#define CMD_LED_OFF _IO ('k', 1)

int main(void) 
{
	int led_fd = open ("/dev/led0", O_RDWR) ;
	if (led_fd < 0) 
		perror ("OPEN DEVICE ERROR!\n");

	while (1) {
		ioctl (led_fd, CMD_LED_ON,  0);
		ioctl (led_fd, CMD_LED_OFF, 0) ;
	}

	return 0 ;
}

