#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <string.h>
#include <unistd.h>

#define CMD_LED_ON  _IO ('k', 0)
#define CMD_LED_OFF _IO ('k', 1)

int main(void) 
{
	int led_fd[4] ;
	char device_name [128] ;
	int i ; 

	for (i = 0; i < 4; i++) {
		memset (device_name, 0, sizeof (device_name)) ;
		sprintf (device_name, "/dev/led%d", i) ;
		led_fd [i] = open (device_name, O_RDWR); 
		if (led_fd[i] < 0) {
			perror ("OPEN LED ERROR!") ;
			return 0;
		}
	} 

	while (1) {
		for (i = 0; i < 4; i++) {
			ioctl (led_fd[i], CMD_LED_ON, 0) ;
			sleep (1);
		}	
		
		for (i = 0; i < 4; i++) {
			ioctl (led_fd[i], CMD_LED_OFF, 0) ;
		}
	}

	return 0 ;
}

